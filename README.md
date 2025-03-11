# Simple Language to x86-64 Compiler

This repository contains a compiler that translates a simple programming language into x86-64 Linux assembly language. The compiler uses Top-Down Predictive Parsing to analyze and generate assembly code.

## Features
- Translates a simple language to x86-64 assembly.
- Uses Top-Down Predictive Parsing for syntax analysis.
- Generates assembly code compatible with Linux (NASM).

## Files
- `main.cpp` – Compiler implementation.
- `input.txt` – Contains simple language source code.
- `example.txt` – Example input and corresponding assembly output.

## Usage
1. Compile the compiler:
   ```sh
   g++ -o compiler main.cpp
   ```
2. Run the compiler with an input file:
   ```sh
   ./compiler < input.txt > output.asm
   ```
3. Assemble and run the generated code:
   ```sh
   nasm -f elf64 output.asm -o output.o
   gcc output.o -o output
   ./output
   ```

## Example
**Input (`input.txt`):**
```
PRINT 5 + 3;
```
**Generated Assembly (`output.asm`):**
```assembly
section .text
global _start
_start:
    mov rax, 8
    call print_int
    mov rax, 60
    xor rdi, rdi
    syscall
```

## License
This project is open-source and licensed under the MIT License.

