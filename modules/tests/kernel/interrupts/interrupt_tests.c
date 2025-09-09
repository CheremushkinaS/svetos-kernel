#include <kernel/interrupts/interrupt_tests.h>
#include <kernel/printk.h>
#include <kernel/interrupts/irq.h>

// Объявление функции регистрации обработчика
extern void register_irq_handler(uint32_t irq, void (*handler)(interrupt_frame_t*));

void irq_ack(uint32_t irq) {
    (void)irq;
    // Заглушка
}

void simulate_irq(uint32_t irq) {
    (void)irq;
    // Заглушка
}

void test_irq_handler(interrupt_frame_t *frame) {
    (void)frame;
    printk("IRQ0 triggered!\n");
    irq_ack(0);
}

void interrupts_run_all_tests() {
    printk("Testing interrupts...\n");
    register_irq_handler(0, test_irq_handler);
    simulate_irq(0);
    printk("Interrupt tests completed.\n");
}
