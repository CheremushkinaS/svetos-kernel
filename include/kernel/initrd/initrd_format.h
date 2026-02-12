#ifndef INITRD_FORMAT_H
#define INITRD_FORMAT_H

#include <stdint.h>

#define INITRD_BINARY_MAGIC 0x4D545653 // "SVTM" в little-endian

typedef struct {
    uint32_t magic;
    uint32_t module_count;
} initrd_binary_header_t;

#endif
