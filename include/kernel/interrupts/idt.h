#ifndef IDT_H
#define IDT_H

#include <kernel/interrupts/interrupt_frame.h>
#include <stdint.h>

typedef void (*isr_t)(interrupt_frame_t*);

void idt_init(void);
void register_interrupt_handler(uint8_t int_num, isr_t handler);

#endif
