section .note.GNU-stack noalloc noexec nowrite progbits

section .text
global gdt_flush

gdt_flush:
    mov eax, [esp + 4]  ; Получаем указатель на GDT
    lgdt [eax]          ; Загружаем GDT

    ; Перезагружаем сегментные регистры
    mov ax, 0x10        ; Селектор данных
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Делаем дальний переход для обновления CS
    jmp 0x08:.flush     ; 0x08 - селектор кода
.flush:
    ret
