section .text
global load_idt
global isr0
global isr6
global isr8
global isr13
global isr14

; Функция для загрузки IDT
load_idt:
    mov eax, [esp + 4]  ; Получаем указатель на структуру idt_ptr
    lidt [eax]          ; Загружаем IDT
    ret

; Обработчик исключения деления на ноль
isr0:
    ; Сохраняем только необходимые регистры
    push eax
    push ebx
    
    ; Выводим символ '0' в последовательный порт
    mov dx, 0x3F8
    mov al, '0'
    out dx, al
    
    ; Пропускаем ошибочную инструкцию div (2 байта)
    ; EIP находится по адресу [esp+8] (после push eax и push ebx)
    add dword [esp + 8], 2
    
    ; Восстанавливаем регистры
    pop ebx
    pop eax
    
    ; Возврат из исключения
    iret

; Обработчик неверного опкода
isr6:
    push eax
    push ebx
    
    mov dx, 0x3F8
    mov al, '6'
    out dx, al
    
    ; Пропускаем инструкцию UD2 (2 байта)
    add dword [esp + 8], 2
    
    pop ebx
    pop eax
    iret

; Остальные обработчики пока остаются простыми
isr8:
    push eax
    mov dx, 0x3F8
    mov al, '8'
    out dx, al
    pop eax
    iret

isr13:
    push eax
    mov dx, 0x3F8
    mov al, 'D'
    out dx, al
    pop eax
    iret

isr14:
    push eax
    mov dx, 0x3F8
    mov al, 'P'
    out dx, al
    pop eax
    iret
