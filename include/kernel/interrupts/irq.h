#ifndef IRQ_H
#define IRQ_H

#include <kernel/interrupts/interrupt_frame.h>

typedef void (*irq_handler_t)(interrupt_frame_t*);

void irq_install(void);
void irq_register_handler(int irq, irq_handler_t handler);
void irq_unregister_handler(int irq);
void irq_handler(interrupt_frame_t *frame);
void irq_init(void);
int irq_module_init(void);

#endif
