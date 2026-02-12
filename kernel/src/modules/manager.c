#include <kernel/modules/manager.h>
#include <kernel/mm/mm.h>
#include <kernel/debug.h>
#include <string.h>

static LIST_HEAD(loaded_modules);

void register_kernel_module(struct module* mod) {
    if (!mod) return;

    INIT_LIST_HEAD(&mod->list);
    list_add_tail(&mod->list, &loaded_modules);

    kprintf("Module registered: %s\n", mod->name);
}

int initialize_modules(void) {
    struct list_head *pos;
    int init_count = 0;

    kprintf("Initializing kernel modules...\n");

    // Ручной обход списка
    for (pos = loaded_modules.next; pos != &loaded_modules; pos = pos->next) {
        struct module *mod = (struct module *)pos;
        if (mod->init) {
            kprintf("Initializing module: %s\n", mod->name);
            if (mod->init() == 0) {
                init_count++;
            } else {
                kprintf("Failed to initialize module: %s\n", mod->name);
            }
        }
    }

    kprintf("Total modules initialized: %d\n", init_count);
    return init_count;
}

struct module* find_module(const char* name) {
    struct list_head *pos;

    // Ручной обход списка
    for (pos = loaded_modules.next; pos != &loaded_modules; pos = pos->next) {
        struct module *mod = (struct module *)pos;
        if (strcmp(mod->name, name) == 0) {
            return mod;
        }
    }
    return NULL;
}
