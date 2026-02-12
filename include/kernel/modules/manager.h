#ifndef MODULES_MANAGER_H
#define MODULES_MANAGER_H

#include <kernel/list.h>

// Структура module ДОЛЖНА иметь list как первое поле для правильного приведения типов
struct module {
    struct list_head list;  // ДОЛЖНО БЫТЬ ПЕРВЫМ ПОЛЕМ!
    char name[64];
    void* base_address;
    unsigned int size;
    int (*init)(void);
    void (*exit)(void);
};

void register_kernel_module(struct module* mod);
struct module* find_module(const char* name);
int initialize_modules(void);

#endif
