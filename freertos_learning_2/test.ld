MEMORY
{
    rom : ORIGIN = 0x00000000, LENGTH = 384K
    ram : ORIGIN = 0x20000000, LENGTH = 88K
}

SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom 
    .bss : { *(.bss*) } > ram
}
