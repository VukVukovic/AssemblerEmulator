.extern PRINT_REG
.equ B_value, 5

.global print
.global A_location
.global B_text
.global B_value

.section text:
  print:
    loop_print:
    movb (%r2), PRINT_REG
    add $1, %r2
    cmpb (%r2), $0
    jne loop_print
    ret

.section data:
  A_location: .byte 4
  B_text: .byte 'B', ' ', 'V', 'A', 'L', 'U', 'E', ':', ' ', 0
