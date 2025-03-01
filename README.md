# The bflc framework

## Overview

The BrainFuck-like Languages Compiler (bflc) is a compiler framework that targets
[Brainfuck][brainfuck] and closely [related languages][family].

## Build

Before building, get the source code by cloning the repository.
```sh
git clone --recurse https://github.com/bynect/bflc.git
```

Build the project using CMake.
```sh
cmake -B _build
make -C _build
```

### Note

Guaranteed to work for Linux, requires the Unix Makefile generator for CMake.

## Compiler

The compiler is a terminal interface for the [blfc library](#library).

### Options

- `--debug` Set debug output
- `--verbose` or `-v` Set verbose output
- `--help` or `-h` Print help message
- `--version` Print version message
- `--frontend=STRING` Set one of the [available frontends](#supported-frontends). The default is `brainfuck`
- `--backend=STRING` Set one of the [available backends](#supported-backends). The default is `amd64_asm`
- `--front=STRING` Alternative name for `--frontend` option
- `--back=STRING` Alternative name for `--backend` option
- `-o` Set output file path. The default is `output`
- `-fread` Use read syscall (not available in all backends)
- `-fwrite` Use write syscall (not available in all backends)
- `-fsyscall` Equivalent to `-fread` and `-fwrite`
- `-fcell=INT` Set the number of cells (default is `30000`)

### Example

```sh
# Compile echo.bf to x86_64 nasm assembly
bflc echo.bf -o echo.asm
```

### Note

The `amd64` backend is not supported at the moment.
To create x86\_64 executables assemble the output of `amd64_asm`.

## Library

The framework is by default built as a standalone static library `libbflc.a`,
that can be easily linked with your projects.
The frontends and backends are not included in the static library,
so you must compile them separately.

### Supported frontends

- [Brainfuck][brainfuck] frontend ([`brainfuck`](/front/brainfuck.c))
- [FuckbeEs][fuckbees] frontend ([`fuckbees`](/front/fuckbees.c))

### Supported backends

- [X86\_64 assembly](x64-asm) backend, uses NASM syntax ([`amd64_asm`](/back/amd64_asm.c))
- [X86\_64 machine code](x64-mach) backend, only for JIT-ing ([`amd64`](/back/amd64.c))

### Jit integration

Backends may generate machine code (for example `amd64`).
Since you can use the framework as a library you can create a JIT quite easily.
A practical [integration example](/jit/example.c) is also available.

## Contributing

Contributions are welcome.
Please open an issue first for major changes.

Before pushing contributions, please check for errors, memory leaks and other
problems with either [Asan][asan] or [Valgrind][valgrind].

## License

The bflc framework is licensed under the terms and conditions of the
[Apache-2.0 License](LICENSE).

[family]: https://esolangs.org/wiki/Trivial_brainfuck_substitution
[brainfuck]: https://en.wikipedia.org/wiki/Brainfuck
[fuckbees]: https://esolangs.org/wiki/FuckbeEs
[x64-asm]: https://en.wikipedia.org/wiki/X86_assembly_language
[x64-mach]: https://www.felixcloutier.com/x86
[asan]: https://en.wikipedia.org/wiki/AddressSanitizer
[valgrind]: https://valgrind.org/
