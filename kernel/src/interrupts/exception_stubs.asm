section .note.GNU-stack noalloc noexec nowrite progbits

section .text
global load_idt
global exception_handlers

extern exception_handler

%macro exception_noerr 1
global isr%1
isr%1:
    push byte 0
    push byte %1
    jmp isr_common
%endmacro

%macro exception_err 1
global isr%1
isr%1:
    push byte %1
    jmp isr_common
%endmacro

exception_noerr 0
exception_noerr 1
exception_noerr 2
exception_noerr 3
exception_noerr 4
exception_noerr 5
exception_noerr 6
exception_noerr 7
exception_err   8
exception_noerr 9
exception_err   10
exception_err   11
exception_err   12
exception_err   13
exception_err   14
exception_noerr 15
exception_noerr 16
exception_err   17
exception_noerr 18
exception_noerr 19
exception_noerr 20
exception_noerr 21
exception_noerr 22
exception_noerr 23
exception_noerr 24
exception_noerr 25
exception_noerr 26
exception_noerr 27
exception_noerr 28
exception_noerr 29
exception_err   30
exception_noerr 31

isr_common:
    pusha
    mov ax, ds
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp
    call exception_handler
    add esp, 4
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa
    add esp, 8
    iret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    ret
