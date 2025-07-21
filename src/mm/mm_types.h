#pragma once

#include <stdint.h>
#include <stddef.h>

// Флаги регионов памяти
#define MEM_READ   0x01
#define MEM_WRITE  0x02
#define MEM_EXEC   0x04
#define MEM_DEVICE 0x08  // Для MMIO

typedef struct {
    uintptr_t base;
    size_t size;
    uint32_t flags;
} mm_region_t;

typedef struct {
    uintptr_t *pgtbl;
    mm_region_t *regions;
    size_t region_count;
    uint32_t asid;
} mm_space_t;

// Структура для аллокатора
typedef struct {
    size_t total_blocks;
    size_t free_blocks;
    uint8_t *bitmap;
} buddy_allocator_t;
