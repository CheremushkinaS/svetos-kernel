#ifndef INITRD_H
#define INITRD_H

#include <kernel/types.h>
#include <kernel/initrd/initrd_structs.h>

// Функция для загрузки модулей из initrd по указанному диапазону адресов
int load_modules_from_initrd_range(uint32_t start, uint32_t end);

// Функция для загрузки всех модулей из initrd
int load_modules_from_initrd(void);

// Функция для регистрации модуля в системе
int register_module(const char *name, void *data, uint32_t size);

// Дополнительные функций для работы с initrd
void initrd_init(uint32_t start, uint32_t end);
void initrd_list_files(void);
int initrd_verify_signatures(void);

// Функции для работы с файлами initrd
initrd_file_t* initrd_get_files(void);
uint32_t initrd_get_file_count(void);
initrd_file_t* initrd_find_file(const char* name);

// Функции для получения информации о initrd
uint32_t get_initrd_start(void);
uint32_t get_initrd_end(void);
uint32_t get_initrd_size(void);

#endif // INITRD_H
