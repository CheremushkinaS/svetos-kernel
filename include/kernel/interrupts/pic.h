#ifndef PIC_H
#define PIC_H

#include <kernel/types.h>

// Порты PIC (Programmable Interrupt Controller)
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// Команды инициализации PIC
#define ICW1_INIT       0x10
#define ICW1_ICW4       0x01
#define ICW4_8086       0x01

// Команда конца прерывания (End Of Interrupt)
#define PIC_EOI         0x20

// Прототипы функций
void pic_remap(int offset1, int offset2);
void pic_send_eoi(unsigned char irq);
void pic_set_mask(unsigned char irq_line);
void pic_clear_mask(unsigned char irq_line);

#endif
