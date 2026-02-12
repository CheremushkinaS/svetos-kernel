#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <kernel/interrupts/idt.h>

void exception_handler(interrupt_frame_t* frame);
void exceptions_init(void);
int exceptions_module_init(void);

#endif
