#ifndef IPC_H
#define IPC_H

void ipc_init(void);
int ipc_broadcast(const char* channel_name, const char* message_data);
void ipc_process_messages(void);

#endif
