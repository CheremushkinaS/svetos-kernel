section .multiboot_header
header_start:
    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    dw 0
    dw 0
    dd 8
header_end:

section .text
global start
extern kmain

start:
    cli
    mov esp, stack_top
    push ebx
    push eax
    call kmain
    jmp $

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
