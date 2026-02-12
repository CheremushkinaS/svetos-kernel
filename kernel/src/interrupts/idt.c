#include <stdint.h>
#include <kernel/interrupts/idt.h>
#include <kernel/printk.h>

// Таблица IDT
static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void idt_init(void) {
    asm volatile("cli");

    printk("Initializing IDT...\n");

    // Настраиваем указатель на IDT
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    printk("IDT pointer: base=0x%x, limit=0x%x\n", idt_ptr.base, idt_ptr.limit);

    // Очищаем IDT
    for (int i = 0; i < 256; i++) {
        idt_entries[i].base_low = 0;
        idt_entries[i].base_high = 0;
        idt_entries[i].sel = 0;
        idt_entries[i].always0 = 0;
        idt_entries[i].flags = 0;
    }

    // Устанавливаем обработчики для критических исключений
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);

    // Загружаем IDT
    load_idt(&idt_ptr);
    
    // Включаем прерывания
    asm volatile("sti");

    printk("IDT initialization complete\n");
}
