#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint32_t module_id_t;
typedef uint32_t sanbox_id_t;

typedef struct {
    void* address;
    size_t size;
    module_id_t owner;
} ipc_buffer_t;

typedef struct {
    uint32_t start;
    uint32_t end;
} mm_space_t;
