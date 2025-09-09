#ifndef PIC_H
#define PIC_H

#include <stdint.h>

// Порты PIC
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// Команды инициализации
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

// EOI command
#define PIC_EOI 0x20

// Функции
void pic_remap(int offset1, int offset2);
void pic_set_mask(unsigned char irq_line);
void pic_clear_mask(unsigned char irq_line);
void pic_send_eoi(unsigned char irq);

#endif
