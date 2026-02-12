ENTRY(module_init)
SECTIONS
{
    . = 0x0;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .mod_init : { *(.mod_init) }
    .mod_exit : { *(.mod_exit) }
}
