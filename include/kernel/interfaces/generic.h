#ifndef GENERIC_INTERFACES_H
#define GENERIC_INTERFACES_H

#include <kernel/types.h>

// Универсальные имена интерфейсов
#define INTERFACE_BLOCK_STORAGE    "block_storage"
#define INTERFACE_FILE_SYSTEM      "file_system" 
#define INTERFACE_DISPLAY          "display"
#define INTERFACE_INPUT            "input"
#define INTERFACE_NETWORK          "network"

// Универсальный интерфейс хранилища
struct storage_interface {
    char name[32];
    
    int (*read)(void* device, uint64_t lba, void* buffer, uint32_t count);
    int (*write)(void* device, uint64_t lba, const void* buffer, uint32_t count);
    uint64_t (*get_capacity)(void* device);
    int (*ioctl)(void* device, uint32_t cmd, void* arg);
    
    void* private_data;
};

// Универсальный интерфейс файловой системы
struct filesystem_interface {
    char name[32];
    
    int (*mount)(void* device, const char* options);
    int (*read)(const char* path, void* buffer, uint32_t size, uint32_t offset);
    int (*write)(const char* path, const void* buffer, uint32_t size, uint32_t offset);
    int (*list)(const char* path, char* buffer, uint32_t buffer_size);
    
    void* private_data;
};

// Реестр интерфейсов
void interface_register(const char* name, void* interface);
void* interface_get(const char* name);
int interface_exists(const char* name);

#endif
