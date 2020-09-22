# Linker, section aggregation, global/extern
./../assembler/assembler test6.s -o test6.o -t
./../assembler/assembler test6_1.s -o test6_1.o -t
./../emulator/emulator test6.o test6_1.o -place=ivtp@0x0000
