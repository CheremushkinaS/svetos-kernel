#include <kernel/initrd/initrd.h>
#include <kernel/initrd/initrd_structs.h>
#include <kernel/mm/mm.h>
#include <kernel/modules/manager.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/fs.h>

// Структура заголовка CPIO (формат "newc")
struct cpio_header {
    char c_magic[6];      // Магия "070701" для newc
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
};

// Глобальные переменные для хранения информации о initrd
static uint32_t initrd_start = 0;
static uint32_t initrd_end = 0;
static uint32_t initrd_size = 0;

// Глобальные переменные для хранения информации о файлах initrd
static initrd_file_t* initrd_files = NULL;
static uint32_t initrd_file_count = 0;

// Функция для преобразования восьмеричной строки в число
static uint32_t octal_to_int(char *str, uint32_t len) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '7') {
            result = result * 8 + (str[i] - '0');
        }
    }
    return result;
}

// Функция для выравнивания размера
static uint32_t align_4(uint32_t size) {
    return (size + 3) & ~3;
}

// Функция для проверки контрольной суммы модуля (упрощенная реализация)
static uint32_t calculate_simple_checksum(void *data, uint32_t size) {
    uint32_t checksum = 0;
    uint8_t *bytes = (uint8_t*)data;

    for (uint32_t i = 0; i < size; i++) {
        checksum += bytes[i];
    }

    return checksum;
}

// Функция для проверки сигнатуры модуля
static int verify_module_signature(void *module_data, uint32_t size) {
    // Упрощенная проверка сигнатуры
    // В реальной системе здесь должна быть криптографическая проверка

    // Проверяем минимальный размер модуля
    if (size < 16) {
        printk("Module too small for signature verification\n");
        return 0;
    }

    // Проверяем магическое число в начале модуля
    uint32_t *magic = (uint32_t*)module_data;
    if (*magic != 0x4D4F444D) { // "MODM" в little-endian
        printk("Invalid module magic number: 0x%x\n", *magic);
        return 0;
    }

    // Проверяем контрольную сумму
    uint32_t stored_checksum = *((uint32_t*)(module_data + 8));
    uint32_t calculated_checksum = calculate_simple_checksum(module_data + 16, size - 16);

    if (stored_checksum != calculated_checksum) {
        printk("Module checksum mismatch: stored=0x%x, calculated=0x%x\n",
               stored_checksum, calculated_checksum);
        return 0;
    }

    return 1;
}


// Базовая реализация функций для работы с модулями
int register_module(const char *name, void *data, uint32_t size) {
    (void)data; // Явно помечаем параметр как неиспользуемый
    // Здесь должна быть ваша реализация регистрации модулей
    // Например, вызов функций из module_manager.c
    printk("Module registered: %s, size: %d\n", name, size);
    return 0; // Успех
}

// Функция для загрузки модулей из уже инициализированного initrd

// Функция для получения всех файлов из initrd
initrd_file_t* initrd_get_files(void) {
    return initrd_files;
}

// Функция для получения количества файлов в initrd
uint32_t initrd_get_file_count(void) {
    return initrd_file_count;
}

// Функция для поиска файла по имени
initrd_file_t* initrd_find_file(const char* name) {
    if (initrd_files == NULL || name == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < initrd_file_count; i++) {
        if (strcmp(initrd_files[i].name, name) == 0) {
            return &initrd_files[i];
        }
    }

    return NULL;
}

// Функции для получения информации о initrd
uint32_t get_initrd_start(void) {
    return initrd_start;
}

uint32_t get_initrd_end(void) {
    return initrd_end;
}

uint32_t get_initrd_size(void) {
    return initrd_size;
}

// Функция для инициализации файловой системы initrd
static void initrd_initialize_filesystem(void) {
    if (initrd_start == 0 || initrd_end == 0) {
        printk("Initrd not initialized for filesystem\n");
        return;
    }

    // Освобождаем предыдущие данные, если они есть
    if (initrd_files != NULL) {
        kfree(initrd_files);
        initrd_files = NULL;
        initrd_file_count = 0;
    }

    // Здесь должна быть реализация парсинга initrd и заполнения структур файлов
    // Это упрощенная реализация - в реальной системе нужно парсить CPIO архив
    printk("Initializing initrd filesystem...\n");

    // Временная заглушка - создаем несколько тестовых файлов
    initrd_file_count = 2;
    initrd_files = kmalloc(sizeof(initrd_file_t) * initrd_file_count);

    if (initrd_files != NULL) {
        // Первый файл
        strcpy(initrd_files[0].name, "test.kmod");
        initrd_files[0].size = 1024;
        initrd_files[0].data = kmalloc(initrd_files[0].size);
        initrd_files[0].index = 0;

        // Второй файл
        strcpy(initrd_files[1].name, "vfs_core.kmod");
        initrd_files[1].size = 2048;
        initrd_files[1].data = kmalloc(initrd_files[1].size);
        initrd_files[1].index = 1;

        printk("Initrd filesystem initialized with %d files\n", initrd_file_count);
    } else {
        printk("Failed to allocate memory for initrd files\n");
        initrd_file_count = 0;
    }
}

// Реализации дополнительных функций для работы с initrd
void initrd_init(uint32_t start, uint32_t end) {
    initrd_start = start;
    initrd_end = end;
    initrd_size = end - start;

    printk("Initializing initrd at 0x%x-0x%x (size: %d bytes)\n",
           start, end, initrd_size);

    // Проверяем валидность адресов initrd
    if (start >= end) {
        printk("Warning: Invalid initrd address range\n");
        return;
    }

    // Проверяем, что initrd находится в доступной памяти
    // (здесь может быть дополнительная проверка доступности памяти)

    printk("Initrd initialization completed successfully\n");

    // Инициализируем файловую систему initrd
    initrd_initialize_filesystem();
}

void initrd_list_files(void) {
    if (initrd_start == 0 || initrd_end == 0) {
        printk("Initrd not initialized\n");
        return;
    }

    printk("Listing files in initrd (0x%x-0x%x):\n", initrd_start, initrd_end);

    uint8_t *initrd_ptr = (uint8_t*)initrd_start;
    uint32_t processed = 0;

    while (processed < initrd_size) {
        struct cpio_header *header = (struct cpio_header*)(initrd_ptr + processed);

        // Проверяем магию CPIO
        if (strncmp(header->c_magic, "070701", 6) != 0) {
            printk("Invalid CPIO magic at offset 0x%x\n", processed);
            break;
        }

        // Получаем размер имени файла и размер файла
        uint32_t name_size = octal_to_int(header->c_namesize, 8);
        uint32_t file_size = octal_to_int(header->c_filesize, 8);

        // Вычисляем смещения
        uint32_t header_size = sizeof(struct cpio_header);
        uint32_t name_offset = processed + header_size;
        uint32_t data_offset = name_offset + align_4(name_size);
        uint32_t entry_size = data_offset + align_4(file_size) - processed;

        // Проверяем, не вышли ли за пределы
        if (processed + entry_size > initrd_size) {
            printk("Corrupted CPIO entry at offset 0x%x\n", processed);
            break;
        }

        // Получаем имя файла
        char *filename = (char*)(initrd_ptr + name_offset);

        // Проверяем, является ли это концом архива
        if (strcmp(filename, "TRAILER!!!") == 0) {
            printk("End of archive\n");
            break;
        }

        // Выводим информацию о файле
        uint32_t mode = octal_to_int(header->c_mode, 8);
        if ((mode & 0xF000) == 0x4000) {
            printk("  DIR:  %s\n", filename);
        } else {
            printk("  FILE: %s (%d bytes)\n", filename, file_size);
        }

        processed += entry_size;
    }
}

int initrd_verify_signatures(void) {
    if (initrd_start == 0 || initrd_end == 0) {
        printk("Initrd not initialized\n");
        return 0;
    }

    printk("Verifying module signatures in initrd...\n");

    uint8_t *initrd_ptr = (uint8_t*)initrd_start;
    uint32_t processed = 0;
    int verified_modules = 0;
    int total_modules = 0;

    while (processed < initrd_size) {
        struct cpio_header *header = (struct cpio_header*)(initrd_ptr + processed);

        // Проверяем магию CPIO
        if (strncmp(header->c_magic, "070701", 6) != 0) {
            printk("Invalid CPIO magic at offset 0x%x\n", processed);
            break;
        }

        // Получаем размер имени файла и размер файла
        uint32_t name_size = octal_to_int(header->c_namesize, 8);
        uint32_t file_size = octal_to_int(header->c_filesize, 8);

        // Вычисляем смещения
        uint32_t header_size = sizeof(struct cpio_header);
        uint32_t name_offset = processed + header_size;
        uint32_t data_offset = name_offset + align_4(name_size);
        uint32_t entry_size = data_offset + align_4(file_size) - processed;

        // Проверяем, не вышли ли за пределы
        if (processed + entry_size > initrd_size) {
            printk("Corrupted CPIO entry at offset 0x%x\n", processed);
            break;
        }

        // Получаем имя файла
        char *filename = (char*)(initrd_ptr + name_offset);

        // Проверяем, является ли это концом архива
        if (strcmp(filename, "TRAILER!!!") == 0) {
            break;
        }

        // Пропускаем каталоги
        uint32_t mode = octal_to_int(header->c_mode, 8);
        if ((mode & 0xF000) == 0x4000) {
            processed += entry_size;
            continue;
        }

        // Проверяем только файлы модулей (.kmod)
        if (strstr(filename, ".kmod") != NULL) {
            total_modules++;
            printk("Verifying module: %s\n", filename);

            // Проверяем сигнатуру модуля
            if (verify_module_signature(initrd_ptr + data_offset, file_size)) {
                printk("  Signature verification: PASSED\n");
                verified_modules++;
            } else {
                printk("  Signature verification: FAILED\n");
            }
        }

        processed += entry_size;
    }

    printk("Signature verification completed: %d/%d modules passed\n",
           verified_modules, total_modules);

    return (verified_modules == total_modules) ? 1 : 0;
}
