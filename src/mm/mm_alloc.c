#include "mm_types.h"
#include "../kernel.h"

// Buddy аллокатор для страниц
static buddy_allocator_t buddy_zones[10]; // Для порядков 0-9 (4KB-2MB)

void *alloc_pages(size_t order) {
    if (order > 9) return NULL;
    
    // Поиск свободного блока
    for (size_t i = 0; i < buddy_zones[order].total_blocks; i++) {
        if (!(buddy_zones[order].bitmap[i / 8] & (1 << (i % 8)))) {
            // Помечаем блок как занятый
            buddy_zones[order].bitmap[i / 8] |= (1 << (i % 8));
            buddy_zones[order].free_blocks--;
            return (void*)(ZONE_BASE + (i << (12 + order)));
        }
    }
    return NULL; // Нет свободных блоков
}

void free_pages(void *addr, size_t order) {
    // Рассчет индекса блока
    size_t index = ((uintptr_t)addr - ZONE_BASE) >> (12 + order);
    
    // Освобождение блока
    buddy_zones[order].bitmap[index / 8] &= ~(1 << (index % 8));
    buddy_zones[order].free_blocks++;
}
