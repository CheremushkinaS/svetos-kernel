#ifndef LOADER_H
#define LOADER_H

#include <kernel/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>  // Добавлено для size_t

// Структура для загруженного модуля
typedef struct {
    char name[64];
    void* address;
    uint32_t size;
    uint32_t type;
} loaded_module_t;

// Базовые функции загрузки модулей
int load_module(const char* name);
int unload_module(const char* name);
bool load_module_from_disk(const char* path);
int module_load_from_memory(void* address, size_t size);
int module_unload(const char* name);
int module_resolve_dependencies(void* module);
loaded_module_t* find_loaded_module(const char* name);
void list_loaded_modules(void);

// Функции для работы с initrd
void init_modules(void);
void load_core_modules(void);

#endif
