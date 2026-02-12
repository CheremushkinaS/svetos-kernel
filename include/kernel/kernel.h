#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <multiboot.h>

void kernel_main(unsigned long mbi_addr);

// Внешнее объявление global_mb_info
extern multiboot_info_t* global_mb_info;

#endif
