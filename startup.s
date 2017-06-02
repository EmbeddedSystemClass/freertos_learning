.cpu cortex-m3
.thumb

.thumb_func
.global _start
_start:
    .word 0x20001000
    .word reset

.thumb_func
reset:
    bl main
    b .

