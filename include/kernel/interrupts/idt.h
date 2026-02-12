#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Структура записи IDT
struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// Структура указателя IDT
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct idt_entry idt_entry_t;
typedef struct idt_ptr idt_ptr_t;

// Прототипы функций
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Объявления обработчиков прерываний
extern void isr0(void);   // Division by zero
extern void isr6(void);   // Invalid Opcode
extern void isr8(void);   // Double Fault
extern void isr13(void);  // General Protection Fault
extern void isr14(void);  // Page Fault

// Функция для загрузки IDT
extern void load_idt(idt_ptr_t *ptr);

#endif
