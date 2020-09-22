# Test for arithmetic instructions
./../assembler/assembler test8.s -o test8.o -t
./../emulator/emulator test8.o -place=ivtp@0x0000
