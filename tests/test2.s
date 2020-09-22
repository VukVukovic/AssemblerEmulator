.section ivtp:
  .word main
  .skip 2
  .word timer
  .word keyboard
  .skip 8

.equ PRINT_REG, 0xFF00
.equ TIMER_CFG, 0xFF10
.equ INPUT_REG, 0xFF02

.section text:
  main:
    mov $0x3, TIMER_CFG
    mov $info_label, %r2
    call print
  inf_loop:
    jmp inf_loop
  keyboard:
    push %r2
    mov INPUT_REG, %r2
    sub $'0', %r2
    mov %r2, TIMER_CFG
    pop %r2
    iret
  timer:
    push %r2
    mov $tick_label, %r2
    call print
    addb $1, tick_cnt
    movb tick_cnt, %r2l
    addb $'0', %r2l
    movb %r2l, PRINT_REG
    movb $'\n', PRINT_REG
    pop %r2
    iret
  print:
    loop_print:
    movb (%r2), PRINT_REG
    add $1, %r2
    cmpb (%r2), $0
    jne loop_print
    ret

.section data:
  info_label:
    .byte 'C','H','A','N','G','E',' ','T','I','M','E','R',' ','0','-','7','\n',0
  tick_label:
    .byte 'T', 'I', 'C', 'K', ' ', 0
  tick_cnt: .byte 0
