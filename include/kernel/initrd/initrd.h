#ifndef INITRD_H
#define INITRD_H

#include <stdint.h>
#include <stddef.h>

void initrd_init(uint32_t start, uint32_t end);
void initrd_list_files(void);
int initrd_verify_signatures(void);
void* cpio_get_file(const char* filename, size_t* size);
int load_modules_from_initrd(void);

#endif
