#include <kernel/interrupts/exceptions.h>
#include <kernel/printk.h>
#include <kernel/panic.h>

void exception_handler(interrupt_frame_t* frame) {
    printk("Exception %d occurred\n", frame->int_no);
    printk("Error code: 0x%x\n", frame->err_code);
    printk("EIP: 0x%x, CS: 0x%x\n", frame->eip, frame->cs);
    
    // Для критических исключений вызываем панику
    if (frame->int_no < 20) {
        kernel_panic("Unhandled exception");
    }
}

void exceptions_init(void) {
    printk("Exceptions initialized\n");
}

int exceptions_module_init(void) {
    exceptions_init();
    return 0;
}
