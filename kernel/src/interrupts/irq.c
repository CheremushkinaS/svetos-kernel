#include <kernel/interrupts/irq.h>
#include <kernel/interrupts/pic.h>
#include <kernel/printk.h>
#include <kernel/hal/port_io.h>

static irq_handler_t irq_handlers[16];

// Default IRQ handler that just acknowledges the interrupt
static void default_irq_handler(interrupt_frame_t *frame) {
    // Acknowledge the interrupt
    if (frame->int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

void irq_install(void) {
    irq_init();

    // Register default handler for all IRQs
    for (int i = 0; i < 16; i++) {
        irq_handlers[i] = default_irq_handler;
    }

    printk("IRQ handlers installed\n");
}

void irq_register_handler(int irq, irq_handler_t handler) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_unregister_handler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = default_irq_handler;
    }
}

void irq_handler(interrupt_frame_t *frame) {
    // Get IRQ number from interrupt number
    int irq = frame->int_no - 32;
    
    // Check if IRQ number is valid
    if (irq < 0 || irq >= 16) {
        printk("Invalid IRQ number: %d (int_no: %d)\n", irq, frame->int_no);
        return;
    }

    // Call the registered handler
    if (irq_handlers[irq]) {
        irq_handlers[irq](frame);
    } else {
        printk("Unhandled IRQ: %d\n", irq);
        // Acknowledge the interrupt
        if (irq >= 8) {
            outb(0xA0, 0x20);
        }
        outb(0x20, 0x20);
    }
}

void irq_init(void) {
    // Initialize PIC
    pic_remap(0x20, 0x28);

    // Clear all handlers
    for (int i = 0; i < 16; i++) {
        irq_handlers[i] = NULL;
    }

    printk("IRQ system initialized\n");
}

int irq_module_init(void) {
    irq_init();
    return 0;
}
