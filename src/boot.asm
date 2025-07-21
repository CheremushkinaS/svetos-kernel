section .multiboot_header
header_start:
    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; Добавляем тег выравнивания
    align 8
    dw 0    ; Type
    dw 0    ; Flags
    dd 8    ; Size
header_end:

section .text
global start
extern kmain
extern stack_top  ; Явное объявление внешнего символа

start:
    cli
    ; Используем стек из скрипта линковки
    mov esp, stack_top
    ; Передаем параметры GRUB
    push ebx  ; Multiboot info structure
    push eax  ; Magic number
    call kmain
    cli
    hlt
    jmp $     ; Бесконечный цикл после возврата

section .note.GNU-stack noalloc noexec nowrite progbits
