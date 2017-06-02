MEMORY
{
    rom : ORIGIN = 0x00000000, LENGTH = 512K
    ram : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom 
    .bss : { *(.bss*) } > ram
}