#ifndef INITRD_H
#define INITRD_H

#include <kernel/types.h>
#include <stdint.h>

typedef struct {
    char name[256];
    uint8_t *data;
    uint32_t size;
} initrd_file_t;

void initrd_init(uint32_t start, uint32_t end);
initrd_file_t *initrd_get_files(void);
int initrd_get_file_count(void);
initrd_file_t *initrd_find_file(const char *name);

#endif
