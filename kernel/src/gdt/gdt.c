#include <kernel/gdt.h>
#include <kernel/printk.h>

// Таблица GDT
gdt_entry_t gdt_entries[3];
gdt_ptr_t gdt_ptr;

// Установка записи в GDT
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].access = access;
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= (gran & 0xF0);
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
}

// Внешняя функция для перезагрузки сегментных регистров
extern void gdt_flush(uint32_t);

// Инициализация GDT
void init_gdt(void) {
    // Устанавливаем указатель на GDT
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Нулевой дескриптор (обязателен)
    gdt_set_gate(0, 0, 0, 0, 0);

    // Сегмент кода
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Сегмент данных
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Загрузка GDT
    __asm__ volatile("lgdt (%0)" : : "r" (&gdt_ptr));

    // Перезагрузка сегментных регистров через ассемблерную функцию
    gdt_flush((uint32_t)&gdt_ptr);
}

// Алиас для совместимости с kernel.c
void gdt_init(void) {
    printk("GDT initialization\n");
    init_gdt();
}
