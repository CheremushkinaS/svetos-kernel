#ifndef INTERRUPT_FRAME_H
#define INTERRUPT_FRAME_H

#include <stdint.h>

// Структура должна точно соответствовать порядку сохранения регистров в exception_stubs.asm
typedef struct {
    uint32_t ds;                 // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;   // Interrupt number and error code
    uint32_t eip, cs, eflags;    // Pushed by the processor
    uint32_t user_esp, user_ss;  // User stack (if privilege change)
} interrupt_frame_t;

#endif
