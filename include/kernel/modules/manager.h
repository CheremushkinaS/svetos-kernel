#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <kernel/types.h>
#include <stdint.h>

typedef struct {
    char name[32];
    void *data;
    size_t size;
    uint32_t loaded;
    uint32_t profile_id;
} module_t;

void module_manager_init(void);
int module_load(const char *name, void *data, size_t size);
int module_load_critical(void);
int module_load_from_fs(const char* path);
module_t *module_get(const char *name);
int module_unload(const char *name);

#endif
