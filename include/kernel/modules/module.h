#ifndef MODULE_H
#define MODULE_H

#include <kernel/types.h>
#include <stdint.h>

#define MODULE_NAME_LEN 32
#define MAX_DEPENDENCIES 16
#define MAX_CAPABILITIES 16
#define MAX_MODULES 16

typedef struct {
    char name[MODULE_NAME_LEN];
    module_type_t type;
    uint8_t num_dependencies;
    char dependencies[MAX_DEPENDENCIES][MODULE_NAME_LEN];
    int (*init)(void);
    int (*deinit)(void);
    uint32_t capabilities;
} module_manifest_t;

typedef struct {
    char name[MODULE_NAME_LEN];
    module_type_t type;
    int (*init)(void);
    int (*deinit)(void);
    uint32_t capabilities;
} module_t;

int module_register(const module_manifest_t *manifest);
void module_system_init(void);
module_t* module_find(const char* name);
int module_load_dependencies(module_t* module);
int module_load(const char* name);

#endif
