#ifndef ISR_H
#define ISR_H

#include "idt.h"  // Используем относительный путь

void isr_install(void);
void exception_handler(interrupt_frame_t *frame);

#endif
