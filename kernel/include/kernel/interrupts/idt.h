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

// Структура указателя на IDT
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Структура регистров при прерывании (должна совпадать с той, что используется в exceptions.c)
struct interrupt_frame {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, user_esp, ss;
};

typedef struct idt_entry idt_entry_t;
typedef struct idt_ptr idt_ptr_t;
typedef struct interrupt_frame interrupt_frame_t;

// Функции
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
