#include <kernel/modules/manager.h>
#include <kernel/printk.h>
#include <kernel/initrd/initrd.h>
#include "string.h"

static module_t modules[64];
static int module_count = 0;

void module_manager_init(void) {
    module_count = 0;
    memset(modules, 0, sizeof(modules));
    printk("Module manager initialized\n");
}

int module_load(const char *name, void *data, size_t size) {
    if (module_count >= 64) {
        printk("Module limit reached\n");
        return 0;
    }

    for (int i = 0; i < 64; i++) {
        if (modules[i].loaded == 0) {
            strncpy(modules[i].name, name, 31);
            modules[i].name[31] = '\0';
            modules[i].data = data;
            modules[i].size = size;
            modules[i].loaded = 1;
            modules[i].profile_id = 0;

            module_count++;
            printk("Module loaded: %s (%d bytes)\n", name, size);
            return 1;
        }
    }

    printk("Failed to load module: %s\n", name);
    return 0;
}

int module_load_critical(void) {
    printk("Loading critical modules...\n");

    int loaded_count = 0;

    // Загружаем только самые базовые модули
    loaded_count += module_load("vfs_core", NULL, 0);
    loaded_count += module_load("module_loader", NULL, 0);

    printk("Critical modules loaded: %d\n", loaded_count);
    return loaded_count;
}

int module_load_from_fs(const char* path) {
    printk("Loading modules from filesystem: %s\n", path);
    printk("Filesystem module loading not implemented yet\n");
    return 0;
}

module_t *module_get(const char *name) {
    for (int i = 0; i < 64; i++) {
        if (modules[i].loaded && strcmp(modules[i].name, name) == 0) {
            return &modules[i];
        }
    }
    return 0;
}

int module_unload(const char *name) {
    for (int i = 0; i < 64; i++) {
        if (modules[i].loaded && strcmp(modules[i].name, name) == 0) {
            modules[i].loaded = 0;
            module_count--;
            printk("Module unloaded: %s\n", name);
            return 1;
        }
    }
    return 0;
}
