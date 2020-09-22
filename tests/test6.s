.section ivtp:
  .word main
  .skip 2
  .word timer
  .word keyboard
  .skip 8

.extern print
.extern A_location
.extern B_value
.extern B_text

.global PRINT_REG

.equ PRINT_REG, 0xFF00

.section text:
  keyboard:
    iret
  timer:
    iret
  main:
    mov $A_text, %r2
    call print
    movb A_location, %r3l
    addb $'0', %r3l
    movb %r3l, PRINT_REG
    movb $'\n', PRINT_REG
    mov $B_text, %r2
    call print
    movb $B_value, %r3l
    addb $'0', %r3l
    movb %r3l, PRINT_REG
    movb $'\n', PRINT_REG
    halt

.section data:
  A_text: .byte 'A', ' ', 'V', 'A', 'L', 'U', 'E', ':', ' ', 0
