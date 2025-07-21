#include "mm_types.h"
#include "../kernel.h"

// Статическая таблица страниц ядра
static uint32_t kernel_page_directory[1024] __attribute__((aligned(4096)));

void paging_init() {
    // Настройка identity mapping для нижних 4 МБ
    for (int i = 0; i < 1024; i++) {
        kernel_page_directory[i] = (i * 0x1000) | 0x83; // Large pages, RW, Present
    }
    
    // Загрузка CR3 (x86) или TTBR0 (ARM)
    set_page_directory((uintptr_t)kernel_page_directory);
    enable_paging();
}

int map_region(mm_space_t *space, uintptr_t virt, uintptr_t phys, size_t size, uint32_t flags) {
    // Реализация отображения страниц
    for (size_t i = 0; i < size; i += 4096) {
        // Заполнение PTE
        uint32_t *pte = get_pte(space->pgtbl, virt + i);
        *pte = (phys + i) | flags | 0x01; // Present flag
    }
    return 0;
}
