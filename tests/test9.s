.section ivtp:
  .word main
  .skip 2
  .word routine
  .word routine
  .word 8

.equ PRINT_REG, 0xFF00
.equ A, 2

.section text:
  routine:
    iret

  main:
    mov $1, %r1
    call print_digit

    mov $A, %r1
    call print_digit

    mov $3, %r2
    mov %r2, %r1
    call print_digit

    mov $B, %r2
    mov (%r2), %r1
    call print_digit

    mov $B, %r2
    mov 2(%r2), %r1
    call print_digit

    mov $4, %r2
    mov B(%r2), %r1
    call print_digit

    mov C(%pc), %r1
    call print_digit

    mov $8, 0x0200
    mov 0x200, %r1
    call print_digit

    mov $9, F
    mov F, %r1
    call print_digit

    mov $0x0403, %r2

    mov $0, %r1
    mov %r2h, %r1l
    call print_digit

    mov $0, %r1
    movb %r2l, %r1l
    call print_digit

    halt
  print_digit:
    add $'0', %r1
    mov %r1, PRINT_REG
    mov $'\n', PRINT_REG
    ret

.section data:
  E: .skip2
  B: .word 4
  .word 5
  .word 6
  C: .word 7
  F: .skip 2
