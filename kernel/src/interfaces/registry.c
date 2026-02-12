#include <kernel/interfaces/generic.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/mm/kmalloc.h>

struct interface_entry {
    char name[32];
    void* interface;
    struct interface_entry* next;
};

static struct interface_entry* interfaces = NULL;

void interface_register(const char* name, void* interface) {
    if (!name || !interface) return;
    
    struct interface_entry* entry = kmalloc(sizeof(struct interface_entry));
    if (!entry) {
        printk("Failed to allocate memory for interface registry\n");
        return;
    }
    
    strncpy(entry->name, name, sizeof(entry->name) - 1);
    entry->name[sizeof(entry->name) - 1] = '\0';
    entry->interface = interface;
    entry->next = interfaces;
    interfaces = entry;
    
    printk("Interface registered: %s\n", name);
}

void* interface_get(const char* name) {
    if (!name) return NULL;
    
    struct interface_entry* current = interfaces;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->interface;
        }
        current = current->next;
    }
    return NULL;
}

int interface_exists(const char* name) {
    return interface_get(name) != NULL;
}
