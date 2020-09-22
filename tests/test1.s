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
    mov $hello_label, %r2
    call print
  inf_loop:
    jmp inf_loop
  keyboard:
    push %r2
    mov $input_label, %r2
    call print
    pop %r2
    movb INPUT_REG, PRINT_REG
    movb $'\n', PRINT_REG
    cmpb 0xFF02, $'x'
    jeq end
    iret
    end:
    mov $byebyte_label, %r2
    call print
    halt
  timer:
    push %r2
    mov $tick_label, %r2
    call print
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
  hello_label:
    .byte 'H', 'E', 'L', 'L', 'O', '!', '\n'
    .byte 'C', 'L', 'I', 'C', 'K', ' ', 'x', ' ', 'T', 'O', ' ', 'E', 'X', 'I', 'T', '\n', 0
  input_label:
    .byte 'C', 'L', 'I', 'C', 'K', 'E', 'D', ':', ' ', 0
  tick_label:
    .byte 'T', 'I', 'C', 'K', '\n', 0
  byebyte_label:
    .byte 'B', 'Y', 'E', ' ', 'B', 'Y', 'E', '!', '\n', 0
