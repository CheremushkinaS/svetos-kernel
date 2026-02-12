#include <kernel/module.h>
#include <kernel/interfaces/generic.h>

static struct filesystem_interface initrd_fs;

static int initrd_mount(void* device, const char* options) {
    (void)device;   // Unused parameter
    (void)options;  // Unused parameter
    return 0;
}

static int initrd_read(const char* path, void* buffer, uint32_t size, uint32_t offset) {
    (void)path;    // Unused parameter
    (void)buffer;  // Unused parameter  
    (void)size;    // Unused parameter
    (void)offset;  // Unused parameter
    return -1; // Not implemented yet
}

static int initrd_write(const char* path, const void* buffer, uint32_t size, uint32_t offset) {
    (void)path;    // Unused parameter
    (void)buffer;  // Unused parameter
    (void)size;    // Unused parameter
    (void)offset;  // Unused parameter
    return -1; // Read-only for now
}

static int initrd_list(const char* path, char* buffer, uint32_t buffer_size) {
    (void)path;        // Unused parameter
    (void)buffer;      // Unused parameter
    (void)buffer_size; // Unused parameter
    return 0; // Empty for now
}

static void initrd_init(void) {
    // Инициализация будет выполнена ядром
}

static void initrd_exit(void) {
    // Деинициализация будет выполнена ядром
}

static void initrd_register_interfaces(void) {
    initrd_fs.name = INTERFACE_FILE_SYSTEM;
    initrd_fs.mount = initrd_mount;
    initrd_fs.read = initrd_read;
    initrd_fs.write = initrd_write;
    initrd_fs.list = initrd_list;
    initrd_fs.private_data = NULL;
    
    interface_register(INTERFACE_FILE_SYSTEM, &initrd_fs);
}

// Структура модуля
struct module initrd_fs_module = {
    .magic = MODULE_MAGIC,
    .name = "initrd_filesystem",
    .version = 0x100,
    .type = MODULE_TYPE_FS,
    .register_interfaces = initrd_register_interfaces,
    .init = initrd_init,
    .exit = initrd_exit
};
