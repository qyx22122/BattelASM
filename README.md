# BattelASM

BattelASM (Brute-forced Antipathy Technically Taxing Elaborate Loop) is a 16-bit virtual machine designed to run two or more programs. The goal of these programs is to be the last one to be alive. Each program starts with 100 HP, and every instruction costs 1 HP. Programs can reset their HP back to 100 by executing the `flag` instruction. All memory can be read from and written to by every program.

## Architecture

The virtual machine is built around a custom RISC architecture defined in `arch.ods`. Every instruction is 16 bits long. Programs are not executed in parallel; instead, they take turns.

To write your own bot, it is strongly recommended to read `main.c`, especially the `process_instruction` function.

## Assembler

- [Assembler by Gregorcnik](https://github.com/Gregorcnik/battelCompile)
