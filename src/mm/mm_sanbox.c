#include "mm_types.h"
#include "mm_api.h"
#include "../module.h"

mm_space_t *create_sandbox(module_id_t mod) {
    mm_space_t *space = kmalloc(sizeof(mm_space_t));
    space->pgtbl = alloc_page(); // Выделяем страницу для таблицы страниц
    space->regions = NULL;
    space->region_count = 0;
    space->asid = allocate_asid(); // Уникальный ASID
    
    // Регистрируем в глобальной таблице ядра
    kernel_add_mm_space(mod, space);
    return space;
}

void destroy_sandbox(module_id_t mod) {
    mm_space_t *space = kernel_get_mm_space(mod);
    
    // Освобождаем все ресурсы
    for (size_t i = 0; i < space->region_count; i++) {
        free_pages((void*)space->regions[i].base, 
                  size_to_order(space->regions[i].size));
    }
    kfree(space);
}
