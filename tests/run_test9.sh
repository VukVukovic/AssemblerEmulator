# Test for addressing types and instruction size
./../assembler/assembler test9.s -o test9.o -t
./../emulator/emulator test9.o -place=ivtp@0x0000
