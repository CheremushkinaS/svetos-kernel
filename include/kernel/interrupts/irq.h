#ifndef IRQ_H
#define IRQ_H

#include <kernel/types.h>

// Определение типа для обработчика прерываний
typedef void (*irq_handler_t)(void);

// Структура фрейма прерывания (упрощенная версия)
typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} interrupt_frame_t;

// Прототипы функций
void irq_init(void);
void irq_register_handler(int irq, irq_handler_t handler);
void irq_unregister_handler(int irq);
void irq_handler(interrupt_frame_t *frame);

#endif
