#include <kernel/interrupts/isr.h>
#include <kernel/printk.h>

void isr_install(void) {
    printk("ISR: Using simple stub handlers\n");
}
