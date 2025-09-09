ENTRY(start)

SECTIONS {
    /* Ядро загружается по адресу 1 МБ */
    . = 1M;

    /* Multiboot-заголовок должен быть первым и без выравнивания */
    .multiboot 0x100000 : {
        *(.multiboot)
    }

    /* Остальные секции с выравниванием */
    .text : ALIGN(4K) {
        *(.text)
    }

    .rodata : ALIGN(4K) {
        *(.rodata)
    }

    .data : ALIGN(4K) {
        *(.data)
    }

    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss)
        . = ALIGN(16);
        stack_bottom = .;
        . += 16K; /* 16KB стек */
        stack_top = .;
    }

    _kernel_end = .;
}
