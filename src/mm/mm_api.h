#pragma once

#include "mm_types.h"

// Публичное API для модулей
void *mod_alloc(size_t size, uint32_t flags);
void mod_free(void *ptr);
uintptr_t mod_get_phys_addr(void *virt);

// API для ядра
mm_space_t *create_sandbox(module_id_t mod);
void destroy_sandbox(module_id_t mod);
ipc_buffer_t *create_shared_buffer(module_id_t src, module_id_t dst, size_t size);
