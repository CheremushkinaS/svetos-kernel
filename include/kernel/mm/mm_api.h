#pragma once

#include <kernel/mm/mm_types.h>
#include <stddef.h>

// Публичный API
void* mod_alloc(size_t size);
void mod_free(void* ptr);

ipc_buffer_t* create_shared_buffer(module_id_t src, module_id_t dst, size_t size);
void destroy_shared_buffer(ipc_buffer_t* buf);

mm_space_t* create_sandbox(module_id_t mod);
void destroy_sandbox(module_id_t mod);

// Функции для ядра
void* kalloc(size_t size);
void kfree(void* ptr);

// Инициализация памяти
void mm_init(void);

// Диагностика
void mm_dump_map(void);
