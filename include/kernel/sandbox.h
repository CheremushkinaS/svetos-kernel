#ifndef SANDBOX_H
#define SANDBOX_H

#include <kernel/types.h>
#include <stdint.h>

// Типы capabilities (прав доступа)
typedef enum {
    CAP_NONE = 0,
    CAP_IO_ACCESS,      // Доступ к портам ввода-вывода
    CAP_INTERRUPT,      // Работа с прерываниями
    CAP_MEM_ALLOC,      // Выделение памяти
    CAP_THREAD_CREATE,  // Создание потоков
    CAP_DEVICE_ACCESS,  // Доступ к устройствам
    CAP_NETWORK,        // Сетевой доступ
    CAP_FILESYSTEM,     // Доступ к файловой системе
    CAP_SYSTEM_CALL     // Возможность системных вызовов
} capability_t;

// Инициализация системы песочницы
void sandbox_init(void);

// Проверка прав доступа модуля
int sandbox_check_permissions(const char* module_name, uint32_t size);

// Создание профиля безопасности для модуля
int sandbox_create_profile(const char* module_name, uint32_t capabilities, uint32_t memory_limit);

// Удаление профиля безопасности
void sandbox_destroy_profile(uint32_t profile_id);

// Выполнение функции инициализации модуля
int sandbox_execute_init(const char* module_name);

// Выполнение функции деинициализации модуля
void sandbox_execute_deinit(const char* module_name);

// Преобразование списка capabilities из JSON в битовую маску
uint32_t sandbox_parse_capabilities(const char* cap_list);

// Проверка наличия capability у модуля
int sandbox_check_capability(uint32_t module_id, capability_t cap);

// Выделение памяти в контексте модуля
void* sandbox_alloc(uint32_t module_id, size_t size);

// Освобождение памяти модуля
void sandbox_free(uint32_t module_id, void* ptr);

// Полная очистка памяти модуля
void sandbox_cleanup_module(uint32_t module_id);

// Получение ID модуля по имени
uint32_t sandbox_get_module_id(const char* module_name);

// Проверка доступа к порту ввода-вывода
int sandbox_check_io_access(uint32_t module_id, uint16_t port);

// Проверка доступа к прерываниям
int sandbox_check_interrupt_access(uint32_t module_id, uint8_t irq);

// Проверка доступа к устройству
int sandbox_check_device_access(uint32_t module_id, const char* device_name);

#endif
