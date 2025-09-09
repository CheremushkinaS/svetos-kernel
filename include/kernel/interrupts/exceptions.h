#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <kernel/interrupts/interrupt_frame.h>

void exceptions_init(void);
int exceptions_module_init(void);
void exception_handler(interrupt_frame_t* frame);

#endif
