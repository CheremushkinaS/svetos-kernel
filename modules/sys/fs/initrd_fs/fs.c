#include <stdbool.h>
#include "fs.h"
#include "string.h"
#include "printk.h"

// Определение структуры dirent
struct dirent {
    char d_name[256];
    int d_type;
};

// Временные заглушки для директорий (помечены как unused)
static struct dirent root_entries[] __attribute__((unused)) = {
    {.d_name = "modules", .d_type = 2},
    {.d_name = "drivers", .d_type = 2},
    {.d_name = "tests", .d_type = 2}
};

static struct dirent modules_entries[] __attribute__((unused)) = {
    {.d_name = "test_module", .d_type = 1},
    {.d_name = "interrupt_tests", .d_type = 1},
    {.d_name = "mm_tests", .d_type = 1},
    {.d_name = "fs_tests", .d_type = 1}
};

static struct dirent drivers_entries[] __attribute__((unused)) = {
    {.d_name = "keyboard", .d_type = 1},
    {.d_name = "timer", .d_type = 1},
    {.d_name = "vga", .d_type = 1}
};

static struct dirent test_entries[] __attribute__((unused)) = {
    {.d_name = "test1", .d_type = 1},
    {.d_name = "test2", .d_type = 1}
};

static struct dirent tests_entries[] __attribute__((unused)) = {
    {.d_name = "kernel", .d_type = 2},
    {.d_name = "user", .d_type = 2}
};

// Минимальная реализация read-only файловой системы
static fs_node_t root_node = {
    .name = "/",
    .type = FS_DIRECTORY,
    .size = 0
};

static fs_node_t modules_node = {
    .name = "/modules",
    .type = FS_DIRECTORY,
    .size = 0
};

static fs_node_t drivers_node = {
    .name = "/modules/drivers",
    .type = FS_DIRECTORY,
    .size = 0
};

static fs_node_t test_node = {
    .name = "/modules/test",
    .type = FS_DIRECTORY,
    .size = 0
};

static fs_node_t tests_node = {
    .name = "/modules/tests",
    .type = FS_DIRECTORY,
    .size = 0
};

static int readdir_index = 0;
static fs_node_t* current_dir = NULL;

bool fs_init(void) {
    printk("Initializing read-only filesystem stub...\n");
    return true;
}

fs_node_t* fs_open(const char* filename) {
    if (strcmp(filename, "/") == 0) return &root_node;
    if (strcmp(filename, "/modules") == 0) return &modules_node;
    if (strcmp(filename, "/modules/drivers") == 0) return &drivers_node;
    if (strcmp(filename, "/modules/test") == 0) return &test_node;
    if (strcmp(filename, "/modules/tests") == 0) return &tests_node;
    return NULL;
}

void fs_close(fs_node_t* node __attribute__((unused))) {
    // Ничего не делаем - заглушка
}

size_t fs_read(fs_node_t* node __attribute__((unused)), void* buf __attribute__((unused)), size_t size __attribute__((unused))) {
    return 0;
}

struct dirent* fs_readdir(fs_node_t* node) {
    if (node != current_dir) {
        current_dir = node;
        readdir_index = 0;
    }

    // Закомментированный код оставлен для будущего использования
    readdir_index = 0;
    current_dir = NULL;
    return NULL;
}

int fs_create(const char* path, uint32_t type __attribute__((unused))) {
    printk("ERROR: Read-only filesystem - cannot create '%s'\n", path);
    return -1;
}

size_t fs_write(fs_node_t* node __attribute__((unused)), const void* buf __attribute__((unused)), size_t size __attribute__((unused))) {
    printk("ERROR: Read-only filesystem - cannot write to '%s'\n", node->name);
    return 0;
}
