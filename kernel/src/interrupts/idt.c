#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/interrupt_frame.h>
#include <kernel/printk.h>
#include "string.h"

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void idt_load();

// Массив для хранения обработчиков прерываний
static isr_t interrupt_handlers[256] = {0};

// Установка шлюза в IDT
void set_idt_gate(uint8_t num, isr_t handler, uint16_t selector, uint8_t flags) {
    uint32_t base = (uint32_t)handler;

    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags | 0x60; // Всегда устанавливаем биты DPL=0
}

// Регистрация обработчика прерывания
void register_interrupt_handler(uint8_t int_num, isr_t handler) {
    // Сохраняем обработчик в массив
    interrupt_handlers[int_num] = handler;
    // Установка шлюза с флагами:
    // P=1 (присутствует), DPL=0 (уровень привилегий ядра), Type=0xE (32-битный шлюз прерывания)
    set_idt_gate(int_num, handler, 0x08, 0x8E);
}

// Инициализация IDT
void idt_init(void) {
    // Настройка указателя IDT
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    // Очистка IDT
    memset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);

    // Загрузка IDT
    asm volatile("lidt %0" : : "m"(idtp));

    printk("IDT initialized\n");
}

// Обработчик прерываний по умолчанию
void isr_handler(interrupt_frame_t* frame) {
    if (interrupt_handlers[frame->int_no]) {
        interrupt_handlers[frame->int_no](frame);
    } else {
        printk("Unhandled interrupt: %d\n", frame->int_no);
    }
}

int idt_module_init(void) {
    idt_init();
    return 0;
}
