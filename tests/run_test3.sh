# PC relative test
./../assembler/assembler test3.s -o test3.o -t
./../assembler/assembler test3_1.s -o test3_1.o -t
./../emulator/emulator test3.o test3_1.o -place=ivtp@0x0000
