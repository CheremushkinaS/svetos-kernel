#ifndef FS_H
#define FS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    // Структура файлового узла
    char name[256];
    uint32_t size;
    uint32_t type;
} fs_node_t;

#define FS_FILE 1
#define FS_DIRECTORY 2

// Прототипы функций файловой системы
int fs_create(const char* path, uint32_t type);
size_t fs_read(fs_node_t* node, void* buf, size_t size);
size_t fs_write(fs_node_t* node, const void* buf, size_t size);
void fs_close(fs_node_t* node);

#endif
fs_node_t* fs_open(const char* path);
