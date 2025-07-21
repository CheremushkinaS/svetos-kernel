ENTRY(start)

SECTIONS {
    . = 1M;
    
    .multiboot BLOCK(4K) : ALIGN(4K) {
        *(.multiboot_header)
    }
    
    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
        *(.note.GNU-stack)
    }
    
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }
    
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }
    
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
        . = ALIGN(16);
        stack_bottom = .;
        . += 16K;        /* 16KB стек */
        stack_top = .;   /* Экспортируем символ */
    }
    
    /* Экспорт символов стека для ассемблера */
    PROVIDE(stack_top = stack_top);
    PROVIDE(stack_bottom = stack_bottom);
    
    /DISCARD/ : {
        *(.comment)
    }
}
