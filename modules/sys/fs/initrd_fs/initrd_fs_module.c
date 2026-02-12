#include <string.h>
#include <string.h>
#include <kernel/module.h>
#include <kernel/interfaces/generic.h>
#include <kernel/printk.h>
#include <kernel/string.h>

static struct filesystem_interface initrd_fs;
static void* storage_device = NULL;

static int initrd_mount(void* device, const char* options) {
    printk("InitRD FS: Mounting filesystem...\n");
    
    // Get the block storage interface
    struct storage_interface* storage = (struct storage_interface*)interface_get(INTERFACE_BLOCK_STORAGE);
    if (!storage) {
        printk("InitRD FS: ERROR - No block storage interface found!\n");
        return -1;
    }
    
    storage_device = device;
    printk("InitRD FS: Using storage device via %s interface\n", storage->name);
    
    // Test read from disk
    char test_buffer[512];
    if (storage->read && storage->read(storage_device, 0, test_buffer, 1) == 0) {
        printk("InitRD FS: Successfully read sector 0 from disk\n");
    } else {
        printk("InitRD FS: WARNING - Could not read from disk\n");
    }
    
    return 0;
}

static int initrd_read(const char* path, void* buffer, uint32_t size, uint32_t offset) {
    struct storage_interface* storage = (struct storage_interface*)interface_get(INTERFACE_BLOCK_STORAGE);
    if (!storage || !storage->read) {
        printk("InitRD FS: No storage available for reading %s\n", path);
        return -1;
    }
    
    // For demo, read from sector 1 (skip boot sector)
    uint64_t lba = 1 + (offset / 512);
    uint32_t sector_offset = offset % 512;
    
    char sector_buffer[512];
    if (storage->read(storage_device, lba, sector_buffer, 1) == 0) {
        uint32_t copy_size = (size < (512 - sector_offset)) ? size : (512 - sector_offset);
        memcpy(buffer, sector_buffer + sector_offset, copy_size);
        
        printk("InitRD FS: Read %d bytes from %s (LBA: %lld)\n", copy_size, path, lba);
        return copy_size;
    }
    
    return -1;
}

static int initrd_write(const char* path, const void* buffer, uint32_t size, uint32_t offset) {
    printk("InitRD FS: Write to %s (size: %d, offset: %d) - NOT IMPLEMENTED\n", path, size, offset);
    return -1; // Read-only for now
}

static int initrd_list(const char* path, char* buffer, uint32_t buffer_size) {
    // Return a static file list for demo
    const char* file_list = "kernel.bin\ninitrd.img\nreadme.txt\ntest.txt\nconfig.txt\n";
    uint32_t len = strlen(file_list);
    
    if (buffer_size >= len) {
        strcpy(buffer, file_list);
        printk("InitRD FS: Listed directory %s\n", path);
        return len;
    }
    return 0;
}

static void initrd_init(void) {
    printk("InitRD Filesystem initialized as module\n");
    printk("  Requires: %s interface\n", INTERFACE_BLOCK_STORAGE);
}

static void initrd_exit(void) {
    printk("InitRD Filesystem unloaded\n");
}

static void initrd_register_interfaces(void) {
    strcpy(initrd_fs.name, INTERFACE_FILE_SYSTEM);
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
    .type = MODULE_TYPE_TEST,
    .register_interfaces = initrd_register_interfaces,
    .init = initrd_init,
    .exit = initrd_exit
};
