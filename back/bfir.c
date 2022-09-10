#include <stdio.h>
#include <assert.h>

#include "back.h"
#include "bfir.h"

static void bfir_instr(Out_Channel *out, Bfir_Instr *instr) {
	switch (instr->kind) {
		case BFIR_ADD:
			out_print(out, "\tadd %ld\n", instr->arg);
			break;

		case BFIR_ADDP:
			out_print(out, "\taddp %ld\n", instr->arg);
			break;

		case BFIR_READ:
			out_print(out, "\tread\n");
			break;

		case BFIR_WRITE:
			out_print(out, "\twrite %ld\n", instr->arg);
			break;

		case BFIR_JMPF:
			out_print(out, "\tjmpf\n");
			break;

		case BFIR_JMPB:
			out_print(out, "\tjmpb\n");
			break;

		default:
			assert(false && "Unreachable");
	}
}

void bfir_emit(Out_Channel *out, Bfir_Entry *entry, Back_Aux *aux) {
	assert(aux == NULL || aux->sign.quad == bfir_back.sign.quad);

	out_print(out, "; bfir v1\n");
	out_print(out, "entry \"%s\" {\n", entry->name);

	Bfir_Instr *instr = bfir_entry_get(entry, entry->head);
	while (true) {
		bfir_instr(out, instr);
		if (instr->next == 0) break;
		instr = bfir_entry_get(entry, instr->next);
	}

	out_print(out, "}\n");
}

const Back_Info bfir_back = {
	.sign.quad = 0xbbff0000bbff0000,
	.emit_f = bfir_emit,
};
