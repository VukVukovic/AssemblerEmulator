.section ivtp:
  .word main
  .skip 2
  .word timer
  .word keyboard
  .skip 8

.equ PRINT_REG, 0xFF00

.global print_global
.extern print_other_file

.section text:
  keyboard:
    iret
  timer:
    iret
  main:
    mov $hello_label, %r2
    call print
    mov $hello_label, %r2
    call *print_same_section(%pc)
    mov $hello_label, %r2
    call *print_other_section(%pc)
    mov $hello_label, %r2
    call *print_global(%pc)
    call *print_other_file(%pc)

    mov pc_rel_data(%pc), %r2
    add $'0', %r2
    movb %r2l, PRINT_REG
    movb $'\n', PRINT_REG

    add $1, pc_rel_data(%pc)

    mov pc_rel_data(%pc), %r2
    add $'0', %r2
    movb %r2l, PRINT_REG
    movb $'\n', PRINT_REG

    halt
  print:
    loop_print:
    movb (%r2), PRINT_REG
    add $1, %r2
    cmpb (%r2), $0
    jne loop_print
    ret
  print_same_section: .word print

.section data:
  hello_label: .byte 'H', 'E', 'L', 'L', 'O', '\n', 0
  pc_rel_data: .word 2

.section pointers:
  print_other_section: .word print
  print_global: .word print
