#include <kernel/interrupts/irq.h>
#include <kernel/interrupts/pic.h>
#include <kernel/hal/port_io.h>
#include <kernel/printk.h>

// Массив обработчиков прерываний
static irq_handler_t irq_handlers[16];

// Обработчик по умолчанию
static void default_irq_handler(void) {
    printk("Unhandled IRQ occurred!\n");
}

// Установка обработчика прерывания
void irq_register_handler(int irq, irq_handler_t handler) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

// Удаление обработчика прерывания
void irq_unregister_handler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = default_irq_handler;
    }
}

// Основной обработчик прерываний
void irq_handler(interrupt_frame_t *frame) {
    // Вызываем соответствующий обработчик
    if (frame->int_no >= 32 && frame->int_no < 48) {
        int irq_num = frame->int_no - 32;
        if (irq_handlers[irq_num]) {
            irq_handlers[irq_num]();
        }
    }

    // Отправляем EOI (End of Interrupt) в PIC
    pic_send_eoi(frame->int_no - 32);
}

// Инициализация IRQ
void irq_init(void) {
    printk("Initializing IRQ...\n");

    // Инициализируем все обработчики значением по умолчанию
    for (int i = 0; i < 16; i++) {
        irq_handlers[i] = default_irq_handler;
    }

    // Переназначаем PIC
    pic_remap(0x20, 0x28);

    printk("IRQ initialized\n");
}
