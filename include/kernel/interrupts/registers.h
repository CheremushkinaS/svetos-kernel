#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
    // Убраны es, fs, gs так как они не сохраняются
} registers_t;

#endif
