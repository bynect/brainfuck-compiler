#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "back/amd64.h"
#include "front/brainfuck.h"

// TODO: Add an example with mremap

int main(int argc, const char **argv) {
	bool debug = false;

	// Check arguments
	if (argc == 1 || argc > 3) {
		fprintf(stderr, "Usage: %s [--debug] FILE\n", argv[0]);
		return 1;
	}

	const char *path = argv[1];
	if (argc == 3) {
		if (!strcmp(argv[1], "--debug")) {
			debug = true;
			path = argv[2];
		} else {
			fprintf(stderr, "Usage: %s [--debug] FILE\n", argv[0]);
			return 1;
		}
	}

	FILE *file = stdin;
	if (strcmp(path, "-")) {
		file = fopen(argv[1], "rb");
		assert(file != NULL && "Failed to open file");
	}

	// Open input file

	In_Channel in;
	in_init_file(&in, file);

	// Init IR instruction pool
	const size_t instrs_len = 4096;
	Bfir_Instr instrs[instrs_len];

	Bfir_Pool pool;
	bfir_pool_init(&pool, instrs, instrs_len, NULL);

	Bfir_Entry entry;
	bfir_entry_init(&entry, "", &pool);

	// Parse input to IR
	brainfuck_front.parse_f(&in, &entry, NULL);
	if (file != stdin) fclose(file);

	// Init x86_64 backend
	const size_t labels_len = 1024;
	Label_Id labels[labels_len];

	Label_Stack stack1;
	label_stack_init(&stack1, labels, labels_len / 2);

	Label_Stack stack2;
	label_stack_init(&stack2, labels + labels_len / 2, labels_len / 2);

	uint8_t cells[10000];
	Amd64_Layout mem = {
		.cells   = (uint64_t)cells,
		.getchar = (uint64_t)getchar,
		.putchar = (uint64_t)putchar,
	};

	Amd64_Aux aux;
	amd64_aux_init(&aux, &stack1, &stack2, &mem, AMD64_RELATIVE_CALL);

	// Memory map buffer
	const size_t mem_len = 4096;
    void *mem_ptr = mmap(NULL, mem_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mem_ptr == MAP_FAILED || mem_ptr == NULL) {
		perror("mmap");
		return 1;
	}

	Byte_Buffer buffer;
	byte_buffer_init(&buffer, mem_ptr, mem_len);

	Out_Channel out;
	out_init_buffer(&out, &buffer);

	// Emit x86_64 machine code
	amd64_back.emit_f(&out, &entry, (void *)&aux);

	if (debug) {
		printf("Program compiled, %zu bytes emitted\n", buffer.len);
		for (size_t i = 0; i < buffer.len; ++i) printf("%02x ", buffer.bytes[i]);
		printf("\n\n");
	}

	// Execute compiled machine code
	void (*func)() = mem_ptr;
	func();

	// Clean up buffer
	munmap(mem_ptr, mem_len);

	return 0;
}
