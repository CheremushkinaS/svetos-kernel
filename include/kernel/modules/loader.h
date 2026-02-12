#ifndef LOADER_H
#define LOADER_H

#include <stddef.h>
#include <stdint.h>

void* load_elf_module(void* module_data, uint32_t size);
int load_module_from_cpio(const char* filename);
int load_module(void* module_data, const char* name);

#endif
