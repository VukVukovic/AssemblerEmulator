.global print_other_file

.equ PRINT_REG, 0xFF00

.section text:
  print:
    mov $other_file_label, %r2
    loop_print:
    movb (%r2), PRINT_REG
    add $1, %r2
    cmpb (%r2), $0
    jne loop_print
    ret

.section data:
  other_file_label: .byte 'H', 'E', 'L', 'L', 'O', '2', '!', '\n', 0

.section pointers:
  print_other_file: .word print
