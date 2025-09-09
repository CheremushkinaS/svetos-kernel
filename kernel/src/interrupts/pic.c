#include <kernel/interrupts/pic.h>
#include <kernel/hal/port_io.h>

// Переинициализация PIC с новыми базовыми векторами
void pic_remap(int offset1, int offset2) {
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // Начинаем инициализацию
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    
    // Устанавливаем смещения
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    
    // Настраиваем каскадирование
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    
    // Устанавливаем режим
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    
    // Восстанавливаем маски
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_set_mask(unsigned char irq_line) {
    uint16_t port;
    uint8_t value;
    
    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void pic_clear_mask(unsigned char irq_line) {
    uint16_t port;
    uint8_t value;
    
    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
