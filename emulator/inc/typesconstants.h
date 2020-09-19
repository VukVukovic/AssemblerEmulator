#ifndef TYPECONSTANTS_H
#define TYPECONSTANTS_H

enum Register {R0, R1, R2, R3, R4, R5, R6, SP = R6, R7, PC = R7, PSW, REG_NUM};

enum OpCodes {HALT, IRET, RET, INT, CALL,
              JMP, JEQ, JNE, JGT, PUSH,
              POP, XCHG, MOV, ADD, SUB,
              MUL, DIV, CMP, NOT, AND,
              OR, XOR, TEST, SHL, SHR, OP_NUM};

enum AddressType {IMMED, REGDIR, REGIND, REGINDDISP, MEM, ADDR_NUM};

static const int MEM_SIZE = 1<<16;
static const uint16_t DATA_IN = 0xFF00;
static const uint16_t DATA_OUT = 0xFF02;
static const uint16_t TIMER_CFG = 0xFF10;
#endif
