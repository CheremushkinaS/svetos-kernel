ENTRY(start)

SECTIONS {
    /* Ядро загружается по адресу 1 МБ */
    . = 1M;

    /* Сначала размещаем multiboot заголовок */
    .multiboot : {
        *(.multiboot)
    }

    /* Затем остальные секции */
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
