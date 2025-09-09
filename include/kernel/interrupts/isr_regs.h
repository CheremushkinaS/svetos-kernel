#ifndef _ISR_REGS_H
#define _ISR_REGS_H

#include <stdint.h>

struct isr_regs {
    uint32_t ds;                                     // Сегмент данных
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Регистры, сохраненные pusha
    uint32_t int_no, err_code;                       // Номер прерывания и код ошибки
    uint32_t eip, cs, eflags, user_esp, ss;          // Автоматически сохраняются процессором
};

#endif
