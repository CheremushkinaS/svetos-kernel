#ifndef ISR_H
#define ISR_H

#include <kernel/interrupts/interrupt_frame.h>

void isr_install(void);
void page_fault_handler(interrupt_frame_t *frame, uint32_t err_code);

#endif
