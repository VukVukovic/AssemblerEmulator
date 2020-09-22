# Test for branch instructions
./../assembler/assembler test7.s -o test7.o -t
./../emulator/emulator test7.o -place=ivtp@0x0000
