# Test for timer speeds
./../assembler/assembler test2.s -o test2.o
./../emulator/emulator test2.o -place=ivtp@0x0000
