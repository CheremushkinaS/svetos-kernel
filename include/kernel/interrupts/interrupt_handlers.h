#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

#include <kernel/interrupts/interrupt_frame.h>

// Объявляем тип для обработчиков прерываний
typedef void (*isr_t)(interrupt_frame_t*);

// Прототипы функций
void exceptions_install();

#endif
