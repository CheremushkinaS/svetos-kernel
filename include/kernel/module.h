#ifndef MODULE_H
#define MODULE_H

#include <stdint.h>
#include <kernel/types.h>

// Типы функций модуля
typedef int (*module_init_func)(void);
typedef int (*module_deinit_func)(void);
typedef int (*module_test_func)(void);

// Состояния модуля
#define MODULE_STATE_UNLOADED 0
#define MODULE_STATE_LOADED   1
#define MODULE_STATE_ACTIVE   2
#define MODULE_STATE_ERROR    3

// Базовая структура модуля
typedef struct {
    char name[32];
    uint32_t version;
    module_init_func init;
    module_deinit_func deinit;
    module_test_func run_tests;
    uint32_t state;
    uint32_t flags;
} module_t;

// Функции для работы с модулями
int module_load(const char* name, module_init_func init, module_deinit_func deinit, module_test_func run_tests);
int module_unload(const char* name);
module_t* module_find(const char* name);
void module_init(void);
void module_init_system(void);
void module_print_loaded(void);
void module_run_tests(void);
void module_list(void);

#endif
void module_register_external(const char* name, const char* desc, int type, int subtype, int version, int flags, void* init, void* deinit, void* tests);
