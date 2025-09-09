#include <kernel/interrupts/irq.h>
#include <kernel/printk.h>
#include <hal/port_io.h>

// Массив обработчиков IRQ
static irq_handler_t irq_handlers[16] = {0};

void register_irq_handler(uint32_t irq, irq_handler_t handler) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
        printk("Registered handler for IRQ %u\n", irq);
    }
}

// Функция обработки IRQ теперь находится в irq.c
// Чтобы избежать конфликта имён, мы удалили её отсюда
