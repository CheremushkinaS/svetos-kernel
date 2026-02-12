#ifndef SANDBOX_H
#define SANDBOX_H

#include <kernel/types.h>

// Размер памяти для санбокса
#define SANDBOX_MEMORY_SIZE 4096

// Возможности (capabilities) для санбоксов
#define CAP_ACCESS_DISK    (1 << 0)
#define CAP_ACCESS_PIO     (1 << 1)
#define CAP_ACCESS_FS      (1 << 2)
#define CAP_ACCESS_MEMORY  (1 << 3)
#define CAP_SYSCALL        (1 << 4)
#define CAP_IPC            (1 << 5)

// Системные вызовы
#define SYS_IPC_SEND       1

typedef struct {
    int used;
    int id;
    uint32_t capabilities;
    void* memory_ptr;
} sandbox_t;

void sandbox_init(void);
int sandbox_create(void);
int sandbox_destroy(int sandbox_id);
int sandbox_grant_capability(int sandbox_id, uint32_t capability);
int sandbox_revoke_capability(int sandbox_id, uint32_t capability);
int sandbox_check_permission(int sandbox_id, uint32_t required_caps);
void* sandbox_alloc(int sandbox_id, size_t size);
int sandbox_syscall(int sandbox_id, int syscall_num, void* args);

#endif // SANDBOX_H
