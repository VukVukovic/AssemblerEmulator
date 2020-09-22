.equ A, 5
.equ B, A+1
.equ C, A+B-4
.extern D
.equ E, D+C
.equ F, label1-3
.equ G, label1-main
.equ H, label1-main+E
.equ I, label1+3
.global I
.global C

.section text:
  main:
    halt
  label1:
    halt
