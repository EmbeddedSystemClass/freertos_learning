MEMORY
{
    rom : ORIGIN = 0x00000000, LENGTH = 64K
    ram : ORIGIN = 0x10000000, LENGTH = 8K
}

SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom 
    .bss : { *(.bss*) } > ram
}