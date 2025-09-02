# BattelASM
BattelASM is a 16-bit virtual machine designed to run two or more programs. The goal of these programs is to be the last one to survive. Every program starts with 100 HP and each instruction costs it 1 HP. The programs can reset their HP back to 100 by executing the  `flag` instruction. The whole memory can be written to and read from by every program.
## Architecture
The virtual machine is built around a custom RISC arhitecture that is defined in `arch.ods`. Every instruction is 16 bits long. Programs are executed at the same time but not in parallel.
## Assembler
- [Assambler by Gregorcnik](https://github.com/Gregorcnik/battelCompile)
