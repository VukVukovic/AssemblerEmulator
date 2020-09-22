.section ivtp:
  .word main
  .skip 2
  .word routine
  .word routine
  .word 8

.equ PRINT_REG, 0xFF00

.section text:
  routine:
    iret
  main:
    cmp $10, $20
    jgt greater1
    mov $not_greater_label, %r2
    jmp continue1
    greater1:
    mov $greater_label, %r2
    continue1:
    call print

    cmp $20, $-10
    jgt greater2
    mov $not_greater_label, %r2
    jmp continue2
    greater2:
    mov $greater_label, %r2
    continue2:
    call print

    cmp $-200, $-200
    jeq equals1
    mov $not_equals_label, %r2
    jmp continue3
    equals1:
    mov $equals_label, %r2
    continue3:
    call print

    cmp $-200, $482
    jne notequals1
    mov $equals_label, %r2
    jmp continue4
    notequals1:
    mov $not_equals_label, %r2
    continue4:
    call print

.equ C_MASK, 0x0004

    mov $3, %r3
    shr $1, %r3
    mov %psw, %r3
    and $C_MASK, %r3
    cmp $0, %r3
    jeq notset1
    mov $c_bit_set, %r2
    jmp continue5
    notset1:
    mov $c_bit_not_set, %r2
    continue5:
    call print

    mov $2, %r3
    shr $1, %r3
    mov %psw, %r3
    and $C_MASK, %r3
    cmp $0, %r3
    jeq notset2
    mov $c_bit_set, %r2
    jmp continue6
    notset2:
    mov $c_bit_not_set, %r2
    continue6:
    call print

    mov $0xF0, %r3h
    shl $1, %r3h
    mov %psw, %r3
    and $C_MASK, %r3
    cmp $0, %r3
    jeq notset3
    mov $c_bit_set, %r2
    jmp continue7
    notset3:
    mov $c_bit_not_set, %r2
    continue7:
    call print

    halt
  print:
      loop_print:
      movb (%r2), PRINT_REG
      add $1, %r2
      cmpb (%r2), $0
      jne loop_print
      ret

.section data:
  greater_label:
    .byte 'G', 'R', 'E', 'A', 'T', 'E', 'R', '\n', 0
  not_greater_label:
      .byte 'N', 'O', 'T', ' ', 'G', 'R', 'E', 'A', 'T', 'E', 'R', '\n', 0
  equals_label:
    .byte 'E', 'Q', 'U', 'A', 'L', 'S', '\n', 0
  not_equals_label:
    .byte 'N','O','T', ' ', 'E', 'Q', 'U', 'A', 'L', 'S', '\n', 0
  c_bit_set:
    .byte 'C', ' ', 'S', 'E', 'T', '\n', 0
  c_bit_not_set:
    .byte 'C', ' ', 'N', 'O', 'T', ' ', 'S', 'E', 'T', '\n', 0
