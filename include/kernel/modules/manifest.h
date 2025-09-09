#ifndef MODULE_MANIFEST_H
#define MODULE_MANIFEST_H

#include <kernel/types.h>

#define MAX_DEPS 10
#define MAX_CAPS 10

typedef struct {
    char name[32];
    char type[16];
    char version[16];
    char deps[MAX_DEPS][32];
    char caps[MAX_CAPS][32];
    char entry_point[32];
    char exit_point[32];
    uint32_t num_deps;
    uint32_t num_caps;
} module_manifest_t;

int parse_module_manifest(const char *json_data, module_manifest_t *manifest);

#endif
