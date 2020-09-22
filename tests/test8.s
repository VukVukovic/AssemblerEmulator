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
    mov $142, %r0
    call PRINT_NUMBER
    mov $'+', PRINT_REG
    mov $427, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $142, %r0
    add $427, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG

    mov $325, %r0
    call PRINT_NUMBER
    mov $'-', PRINT_REG
    mov $124, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $325, %r0
    sub $124, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG

    mov $21, %r0
    call PRINT_NUMBER
    mov $'*', PRINT_REG
    mov $13, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $21, %r0
    mul $13, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG

    mov $25, %r0
    call PRINT_NUMBER
    mov $'>', PRINT_REG
    mov $'>', PRINT_REG
    mov $1, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $25, %r0
    shr $1, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG

    mov $25, %r0
    call PRINT_NUMBER
    mov $'<', PRINT_REG
    mov $'<', PRINT_REG
    mov $1, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $25, %r0
    shl $1, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG

    mov $11, %r0
    call PRINT_NUMBER
    mov $'^', PRINT_REG
    mov $5, %r0
    call PRINT_NUMBER
    mov $'=', PRINT_REG
    mov $11, %r0
    xor $5, %r0
    call PRINT_NUMBER
    mov $'\n', PRINT_REG
    halt

  PRINT_NUMBER:
    push %r1
    push %r2
    mov $0, %r2
    mov %r0, %r1
    loop: mov %r1, %r0
    call GET_DIGIT
    push %r0
    add $1, %r2
    div $10, %r1
    cmp %r1, $0
    jne loop
    loop2: pop %r0
    call PRINT_DIGIT
    sub $1, %r2
    cmp %r2, $0
    jne loop2
    pop %r1
    pop %r2
    ret

  GET_DIGIT:
    push %r1
    mov %r0, %r1
    div $10, %r1
    mul $10, %r1
    sub %r1, %r0
    pop %r1
    ret

  PRINT_DIGIT:
    add $'0', %r0
    mov %r0, PRINT_REG
    ret
