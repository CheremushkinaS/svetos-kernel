#ifndef MM_TYPES_H
#define MM_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t total_memory;
    size_t free_memory;
} mm_stats_t;

typedef uint32_t module_id_t;

typedef struct {
    void* data;
    size_t size;
} ipc_buffer_t;

typedef struct {
    uintptr_t start;
    size_t size;
    module_id_t owner;
} mm_space_t;

#endif
