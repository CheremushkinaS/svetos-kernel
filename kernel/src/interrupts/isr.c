#include <kernel/interrupts/isr.h>
#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/exceptions.h>
#include <kernel/printk.h>

void isr_install(void) {
    exceptions_init();  // Заменяем exceptions_install на exceptions_init
    idt_init();
    printk("ISR installation completed\n");
}

void page_fault_handler(interrupt_frame_t *frame, uint32_t err_code) {
    printk("Page fault at 0x%x, error code: 0x%x\n", frame->eip, err_code);
    // Здесь должна быть обработка page fault
    while (1); // Зависаем при page fault
}
