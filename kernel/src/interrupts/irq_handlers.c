#include <kernel/interrupts/irq.h>
#include <kernel/printk.h>

// Массив обработчиков IRQ
static irq_handler_t irq_handlers[16] = {0};

void register_irq_handler(uint32_t irq, irq_handler_t handler) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
        printk("Registered handler for IRQ %u\n", irq);
    }
}
