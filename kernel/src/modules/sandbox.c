#include <kernel/sandbox.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/mm/mm.h>
#include <kernel/modules/manager.h>

#define MAX_CAPABILITIES 16
#define MAX_MODULES 32

// Структура профиля безопасности модуля
typedef struct {
    uint32_t module_id;
    char module_name[32];
    uint32_t capabilities;
    uint32_t memory_limit;
    uint32_t memory_used;
    void* memory_base;
} sandbox_profile_t;

static sandbox_profile_t profiles[MAX_MODULES];
static uint32_t profile_count = 0;

// Таблица преобразования имен capabilities в их битовые маски
static struct {
    const char* name;
    capability_t cap;
} cap_table[] = {
    {"io_access", CAP_IO_ACCESS},
    {"interrupt", CAP_INTERRUPT},
    {"mem_alloc", CAP_MEM_ALLOC},
    {"thread_create", CAP_THREAD_CREATE},
    {"device_access", CAP_DEVICE_ACCESS},
    {"network", CAP_NETWORK},
    {"filesystem", CAP_FILESYSTEM},
    {"system_call", CAP_SYSTEM_CALL},
    {NULL, CAP_NONE}
};

// Инициализация системы песочницы
void sandbox_init(void) {
    memset(profiles, 0, sizeof(profiles));
    profile_count = 0;
    printk("Sandbox system initialized\n");
}

// Создание профиля безопасности для модуля
int sandbox_create_profile(const char* module_name, uint32_t capabilities, uint32_t memory_limit) {
    if (profile_count >= MAX_MODULES) {
        printk("Cannot create sandbox profile for %s: maximum profiles reached\n", module_name);
        return -1;
    }
    
    // Находим свободный слот
    uint32_t i;
    for (i = 0; i < MAX_MODULES; i++) {
        if (profiles[i].module_name[0] == '\0') {
            break;
        }
    }
    
    if (i >= MAX_MODULES) {
        printk("No free sandbox profiles available\n");
        return -1;
    }
    
    // Заполняем профиль
    strncpy(profiles[i].module_name, module_name, sizeof(profiles[i].module_name) - 1);
    profiles[i].module_id = i;
    profiles[i].capabilities = capabilities;
    profiles[i].memory_limit = memory_limit;
    profiles[i].memory_used = 0;
    
    // Выделяем память для модуля
    profiles[i].memory_base = kmalloc(memory_limit);
    if (!profiles[i].memory_base) {
        printk("Failed to allocate memory for module %s\n", module_name);
        return -1;
    }
    
    profile_count++;
    printk("Created sandbox profile for %s: capabilities=0x%x, memory=%d bytes\n", 
           module_name, capabilities, memory_limit);
    
    return i;
}

// Преобразование имени capability в битовую маску
static uint32_t cap_name_to_bitmask(const char* cap_name) {
    for (int i = 0; cap_table[i].name != NULL; i++) {
        if (strcmp(cap_table[i].name, cap_name) == 0) {
            return (1 << cap_table[i].cap);
        }
    }
    return 0;
}

// Преобразование списка capabilities из JSON в битовую маску
uint32_t sandbox_parse_capabilities(const char* cap_list) {
    uint32_t result = 0;
    char buffer[256];
    char* token;
    
    strncpy(buffer, cap_list, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    token = strtok(buffer, ",");
    while (token != NULL) {
        // Пропускаем пробелы
        while (*token == ' ') token++;
        
        uint32_t cap_bit = cap_name_to_bitmask(token);
        if (cap_bit) {
            result |= cap_bit;
            printk("  Added capability: %s (0x%x)\n", token, cap_bit);
        } else {
            printk("  Unknown capability: %s\n", token);
        }
        
        token = strtok(NULL, ",");
    }
    
    return result;
}

// Проверка наличия capability у модуля
int sandbox_check_capability(uint32_t module_id, capability_t cap) {
    if (module_id >= MAX_MODULES) {
        return 0;
    }
    
    if (profiles[module_id].module_name[0] == '\0') {
        return 0;
    }
    
    uint32_t cap_bit = (1 << cap);
    if (profiles[module_id].capabilities & cap_bit) {
        return 1;
    }
    
    printk("Module %s does not have capability %d\n", profiles[module_id].module_name, cap);
    return 0;
}

// Выделение памяти в контексте модуля
void* sandbox_alloc(uint32_t module_id, size_t size) {
    if (module_id >= MAX_MODULES) {
        return NULL;
    }
    
    if (profiles[module_id].module_name[0] == '\0') {
        return NULL;
    }
    
    // Проверяем лимит памяти
    if (profiles[module_id].memory_used + size > profiles[module_id].memory_limit) {
        printk("Memory limit exceeded for module %s: %d + %d > %d\n", 
               profiles[module_id].module_name, 
               profiles[module_id].memory_used, 
               size, 
               profiles[module_id].memory_limit);
        return NULL;
    }
    
    // Выделяем память из пула модуля
    void* ptr = (void*)((uint32_t)profiles[module_id].memory_base + profiles[module_id].memory_used);
    profiles[module_id].memory_used += size;
    
    printk("Allocated %d bytes for module %s (total used: %d/%d)\n", 
           size, profiles[module_id].module_name, 
           profiles[module_id].memory_used, 
           profiles[module_id].memory_limit);
    
    return ptr;
}

// Освобождение памяти модуля (в нашей простой реализации память не освобождается индивидуально)
void sandbox_free(uint32_t module_id, void* ptr) {
    // В этой простой реализации мы не реализуем индивидуальное освобождение памяти
    // Вся память освобождается при выгрузке модуля
    (void)ptr; // Подавляем предупреждение о неиспользуемом параметре
    printk("Sandbox free called for module %d\n", module_id);
}

// Полная очистка памяти модуля
void sandbox_cleanup_module(uint32_t module_id) {
    if (module_id >= MAX_MODULES) {
        return;
    }
    
    if (profiles[module_id].module_name[0] == '\0') {
        return;
    }
    
    // Освобождаем память модуля
    if (profiles[module_id].memory_base) {
        kfree(profiles[module_id].memory_base);
        profiles[module_id].memory_base = NULL;
    }
    
    printk("Freed all memory for module %s\n", profiles[module_id].module_name);
    
    // Очищаем профиль
    profiles[module_id].module_name[0] = '\0';
    profiles[module_id].capabilities = 0;
    profiles[module_id].memory_limit = 0;
    profiles[module_id].memory_used = 0;
    
    profile_count--;
}

// Получение ID модуля по имени
uint32_t sandbox_get_module_id(const char* module_name) {
    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (strcmp(profiles[i].module_name, module_name) == 0) {
            return i;
        }
    }
    return MAX_MODULES; // Invalid ID
}

// Проверка доступа к порту ввода-вывода
int sandbox_check_io_access(uint32_t module_id, uint16_t port) {
    if (!sandbox_check_capability(module_id, CAP_IO_ACCESS)) {
        printk("Module %s attempted IO access without capability\n", profiles[module_id].module_name);
        return 0;
    }
    
    // Здесь можно добавить дополнительную логику проверки конкретных портов
    printk("Module %s accessing IO port 0x%x\n", profiles[module_id].module_name, port);
    return 1;
}

// Проверка доступа к прерываниям
int sandbox_check_interrupt_access(uint32_t module_id, uint8_t irq) {
    if (!sandbox_check_capability(module_id, CAP_INTERRUPT)) {
        printk("Module %s attempted interrupt access without capability\n", profiles[module_id].module_name);
        return 0;
    }
    
    // Здесь можно добавить дополнительную логику проверки конкретных прерываний
    printk("Module %s accessing interrupt %d\n", profiles[module_id].module_name, irq);
    return 1;
}

// Проверка доступа к устройству
int sandbox_check_device_access(uint32_t module_id, const char* device_name) {
    if (!sandbox_check_capability(module_id, CAP_DEVICE_ACCESS)) {
        printk("Module %s attempted device access without capability\n", profiles[module_id].module_name);
        return 0;
    }
    
    // Здесь можно добавить дополнительную логику проверки конкретных устройств
    printk("Module %s accessing device %s\n", profiles[module_id].module_name, device_name);
    return 1;
}

// Проверка прав доступа модуля (заглушка)
int sandbox_check_permissions(const char* module_name, uint32_t size) {
    // В реальной системе здесь должна быть проверка цифровой подписи и прав доступа
    (void)module_name; (void)size;
    printk("Sandbox: Checking permissions for module %s (size: %d)\n", module_name, size);
    return 1; // Всегда возвращаем успех для заглушки
}

// Выполнение функции инициализации модуля (заглушка)
int sandbox_execute_init(const char* module_name) {
    // В реальной системе здесь должна быть инициализация модуля в песочнице
    printk("Sandbox: Executing init for module %s\n", module_name);
    return 0; // Всегда возвращаем успех для заглушки
}

// Выполнение функции деинициализации модуля (заглушка)
void sandbox_execute_deinit(const char* module_name) {
    // В реальной системе здесь должна быть деинициализация модуля
    printk("Sandbox: Executing deinit for module %s\n", module_name);
}

// Уничтожение профиля безопасности
void sandbox_destroy_profile(uint32_t profile_id) {
    if (profile_id >= MAX_MODULES) {
        return;
    }

    if (profiles[profile_id].module_name[0] == '\0') {
        return;
    }

    // Освобождаем память модуля
    if (profiles[profile_id].memory_base) {
        kfree(profiles[profile_id].memory_base);
        profiles[profile_id].memory_base = NULL;
    }

    printk("Sandbox: Destroyed profile for module %s\n", profiles[profile_id].module_name);

    // Очищаем профиль
    profiles[profile_id].module_name[0] = '\0';
    profiles[profile_id].capabilities = 0;
    profiles[profile_id].memory_limit = 0;
    profiles[profile_id].memory_used = 0;

    profile_count--;
}
