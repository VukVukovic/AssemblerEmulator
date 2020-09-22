# Int, disabled interrupts
./../assembler/assembler test4.s -o test4.o
./../emulator/emulator test4.o -place=ivtp@0x0000
