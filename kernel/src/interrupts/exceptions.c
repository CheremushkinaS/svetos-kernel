#include <kernel/interrupts/exceptions.h>
#include <kernel/printk.h>

void exception_handler(interrupt_frame_t* frame) {
    printk("Basic exception handler: Exception %d\n", frame->int_no);
    
    // Бесконечный цикл - не пытаемся продолжить выполнение
    while(1) {
        asm volatile("hlt");
    }
}

void exceptions_init(void) {
    printk("Basic exceptions system initialized\n");
}

int exceptions_module_init(void) {
    exceptions_init();
    return 0;
}
