#include <kernel/module.h>
#include <kernel/io.h>
#include <kernel/printk.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static void keyboard_handler() {
    uint8_t status = inb(KEYBOARD_STATUS_PORT);
    if (status & 0x01) {
        uint8_t keycode = inb(KEYBOARD_DATA_PORT);
        printk("Key pressed: 0x%x\n", keycode);
    }
}

int keyboard_init() {
    // Register interrupt handler for keyboard (IRQ1)
    register_irq_handler(1, keyboard_handler);
    printk("Keyboard driver initialized\n");
    return 0;
}

void keyboard_exit() {
    // Unregister handler
    register_irq_handler(1, 0);
}

MODULE_INIT(keyboard_init);
MODULE_EXIT(keyboard_exit);
