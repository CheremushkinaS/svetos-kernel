section .multiboot
align 4

multiboot_header:
    dd 0x1BADB002              ; Magic number
    dd 0x00000003              ; Flags: align modules and provide memory map
    dd -(0x1BADB002 + 0x00000003) ; Checksum

section .text
global start
extern kernel_main

start:
    cli

    ; Устанавливаем стек
    mov esp, stack_top

    ; Передаем параметры GRUB в kernel_main
    push ebx  ; Multiboot info structure (физический адрес)
    push eax  ; Magic number

    call kernel_main

    ; Если kernel_main вернется (чего не должно случиться)
    cli
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB стек
stack_top:
