#ifndef INTERRUPT_FRAME_H
#define INTERRUPT_FRAME_H

#include <stdint.h>

// Структура, сохраняемая процессором при возникновении прерывания
typedef struct {
    uint32_t ds;                 // Сегмент данных
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Регистры общего назначения
    uint32_t int_no, err_code;   // Номер прерывания и код ошибки
    uint32_t eip, cs, eflags;    // Инструкция, сегмент кода и флаги
    uint32_t user_esp, user_ss;  // Стек пользователя (если было переключение)
} interrupt_frame_t;

#endif
