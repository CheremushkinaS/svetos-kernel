#ifndef MODULE_H
#define MODULE_H

#include <stdint.h>
#include <kernel/types.h>

// Тип для IPC сообщений
typedef struct {
    uint32_t type;
    void* data;
    size_t size;
} ipc_message_t;

// Макросы для объявления функций модуля
#define MODULE_INIT(func) int func(void)
#define MODULE_EXIT(func) void func(void)
#define IPC_HANDLER(func) void func(ipc_message_t* msg)

#endif // MODULE_H
