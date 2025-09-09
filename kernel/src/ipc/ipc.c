#include <kernel/ipc.h>
#include <kernel/printk.h>
#include "string.h"

void ipc_init(void) {
    printk("IPC system initialized (simple version)\n");
}

int ipc_broadcast(const char* channel_name, const char* message_data) {
    printk("IPC broadcast: %s - %s\n", channel_name, message_data);
    return 0;
}

void ipc_process_messages(void) {
    // Пустая реализация для начальной загрузки
}
