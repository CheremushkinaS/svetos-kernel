#ifndef MODULE_H
#define MODULE_H

#include <kernel/types.h>

#define MODULE_MAGIC 0x4D4F4455  // "MODU"

struct module {
    uint32_t magic;
    char name[32];
    uint32_t version;
    module_type_t type;
    
    void (*register_interfaces)(void);
    void (*unregister_interfaces)(void);
    void (*init)(void);
    void (*exit)(void);
};

// Функции менеджера модулей
void register_kernel_module(struct module* mod);
void initialize_modules(void);
struct module* find_module(const char* name);

// Совместимость со старым API
typedef struct module module_t;
#define MODULE_UNINITIALIZED 0
#define MODULE_READY 1

void module_manager_init(void);
int module_load_critical(void);
int register_module(const char* name, void* init_func, void* cleanup_func);
int initialize_module(const char* name);
module_t* find_module(const char* name);

#endif
