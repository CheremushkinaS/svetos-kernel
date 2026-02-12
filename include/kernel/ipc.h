#ifndef IPC_H
#define IPC_H

#include <stdint.h>

#define IPC_TYPE_SYSTEM   0
#define IPC_TYPE_DATA     1  
#define IPC_TYPE_COMMAND  2

void ipc_init(void);
int ipc_send(uint32_t to_pid, void* data, uint32_t size, uint32_t type);
int ipc_receive(uint32_t* from_pid, void* buffer, uint32_t size, uint32_t* type);
void ipc_process_messages(void);

// Заглушка если функция не определена
#ifndef get_current_pid
static inline uint32_t get_current_pid(void) { return 0; }
#endif

#endif
