.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
    .word 0x20001000
    .word reset

.thumb_func
reset:
    bl c_entry
    b .

