#ifndef PORT_IO_H
#define PORT_IO_H

#include <kernel/types.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

static inline void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a" (data), "dN" (port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

static inline void outw(uint16_t port, uint16_t data) {
    asm volatile("outw %0, %1" : : "a" (data), "dN" (port));
}

#endif
