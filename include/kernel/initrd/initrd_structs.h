#ifndef INITRD_STRUCTS_H
#define INITRD_STRUCTS_H

#include <kernel/types.h>

// Структура заголовка initrd
typedef struct {
    uint32_t num_files;
    uint32_t file_headers[0];
} initrd_header_t;

// Структура файла в initrd
typedef struct {
    char name[64];
    uint32_t offset;
    uint32_t size;
    uint8_t data[0];
} initrd_file_t;

#endif // INITRD_STRUCTS_H
