#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <kernel/types.h>

typedef struct {
    char name[32];
    int loaded;
    void *data;
    size_t size;
} module_t;

int module_load(const char *name, void *data, size_t size);
module_t *module_get(const char *name);
int module_unload(const char *name);
void module_list(void);

#endif
