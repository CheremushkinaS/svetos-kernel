#include <kernel/modules/loader.h>
#include <kernel/initrd/initrd.h>
#include <kernel/modules/manager.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/mm/mm.h>

// Структура заголовка модуля ядра
typedef struct {
    uint32_t magic;          // Магическое число "MODM"
    uint32_t version;        // Версия модуля
    uint32_t checksum;       // Контрольная сумма
    uint32_t entry_point;    // Точка входа
    char name[64];           // Имя модуля
    char author[64];         // Автор модуля
    char description[128];   // Описание модуля
} __attribute__((packed)) kmod_header_t;

// Функция для проверки заголовка модуля
static int validate_module_header(kmod_header_t* header) {
    if (header->magic != 0x4D4F444D) { // "MODM"
        printk("Invalid module magic: 0x%x\n", header->magic);
        return 0;
    }

    if (header->version == 0) {
        printk("Invalid module version: %d\n", header->version);
        return 0;
    }

    return 1;
}

// Функция для инициализации модуля
static int initialize_module(void* module_data, uint32_t size) {
    if (module_data == NULL || size < sizeof(kmod_header_t)) {
        printk("Invalid module data or size\n");
        return -1;
    }

    kmod_header_t* header = (kmod_header_t*)module_data;

    if (!validate_module_header(header)) {
        printk("Module header validation failed\n");
        return -1;
    }

    printk("Initializing module: %s\n", header->name);
    printk("  Version: %d\n", header->version);
    printk("  Author: %s\n", header->author);
    printk("  Description: %s\n", header->description);

    // Здесь должна быть реальная инициализация модуля
    // Например, вызов точки входа модуля
    if (header->entry_point != 0) {
        // Приводим entry_point к указателю на функцию
        int (*module_init)(void) = (int (*)(void))header->entry_point;

        printk("Calling module entry point at 0x%x\n", header->entry_point);
        int result = module_init();

        if (result != 0) {
            printk("Module initialization failed with code: %d\n", result);
            return -1;
        }
    }

    printk("Module %s initialized successfully\n", header->name);
    return 0;
}

// Загрузка конкретного модуля по имени из initrd
int load_module_from_initrd(const char* name) {
    if (name == NULL) {
        printk("Module name is NULL\n");
        return -1;
    }

    printk("Loading module from initrd: %s\n", name);

    // Используем новую функцию для поиска файла
    initrd_file_t* file = initrd_find_file(name);
    if (file == NULL) {
        printk("Module not found in initrd: %s\n", name);
        return -1;
    }

    printk("Found module %s, size: %d bytes\n", name, file->size);

    // Инициализируем модуль
    if (initialize_module(file->data, file->size) != 0) {
        printk("Failed to initialize module: %s\n", name);
        return -1;
    }

    // Регистрируем модуль в системе
    if (register_module(name, file->data, file->size) != 0) {
        printk("Failed to register module: %s\n", name);
        return -1;
    }

    return 0;
}

// Функция для загрузки всех модулей из initrd
int load_modules_from_initrd(void) {
    int count = 0;
    
    // Получаем список всех файлов в initrd
    initrd_file_t* files = initrd_get_files();
    if (files == NULL) {
        printk("No files found in initrd\n");
        return 0;
    }

    // Загружаем каждый файл с расширением .kmod
    for (int i = 0; files[i].name[0] != '\0'; i++) {
        if (strstr(files[i].name, ".kmod") != NULL) {
            if (load_module_from_initrd(files[i].name) == 0) {
                count++;
            }
        }
    }

    return count;
}
