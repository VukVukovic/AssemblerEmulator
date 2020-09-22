# Test for input/output, timer, interrupts, place
./../assembler/assembler test1.s -o test1.o
./../emulator/emulator test1.o -place=ivtp@0x0000 -place=text@0x1000
