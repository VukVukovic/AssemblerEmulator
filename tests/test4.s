.section ivtp:
  .word main
  .skip 2
  .word timer
  .word keyboard
  .word routine
  .skip 6

.equ PRINT_REG, 0xFF00

.section text:
  timer:
    push %r2
    mov $tick_label, %r2
    call print
    pop %r2
    iret
  keyboard:
    int_loop_keyboard:
    jmp int_loop_keyboard
    iret
  routine:
    push %r2
    mov $routine_label, %r2
    call print
    pop %r2
    iret
  main:
    int $2
    int $4
    inf_loop_main:
    jmp inf_loop_main

  print:
      loop_print:
      movb (%r2), PRINT_REG
      add $1, %r2
      cmpb (%r2), $0
      jne loop_print
      ret

.section data:
  tick_label:
    .byte 'T', 'I', 'C', 'K', '\n', 0
  routine_label:
    .byte 'I', 'N', 'T', ' ', 'R', 'O', 'U', 'T', 'I', 'N', 'E', '\n', 0
