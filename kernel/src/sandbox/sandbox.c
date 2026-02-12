#include <kernel/sandbox/sandbox.h>
#include <kernel/mm/mm.h>
#include <kernel/printk.h>
#include <kernel/string.h>

static sandbox_t sandboxes[32];
static int sandbox_count = 0;

void sandbox_init(void) {
    sandbox_count = 0;
    for (int i = 0; i < 32; i++) {
        sandboxes[i].used = 0;
    }
    printk("Sandbox manager initialized\n");
}

int sandbox_create(void) {
    if (sandbox_count >= 32) {
        printk("Sandbox limit reached\n");
        return -1;
    }

    for (int i = 0; i < 32; i++) {
        if (sandboxes[i].used == 0) {
            sandboxes[i].used = 1;
            sandboxes[i].id = i;
            sandboxes[i].capabilities = 0;
            sandbox_count++;

            // Выделяем изолированную память для санбокса
            sandboxes[i].memory_ptr = kmalloc(SANDBOX_MEMORY_SIZE);
            if (!sandboxes[i].memory_ptr) {
                printk("Failed to allocate memory for sandbox\n");
                sandboxes[i].used = 0;
                sandbox_count--;
                return -1;
            }

            printk("Sandbox created: %d\n", i);
            return i;
        }
    }

    return -1;
}

int sandbox_destroy(int sandbox_id) {
    if (sandbox_id < 0 || sandbox_id >= 32 || !sandboxes[sandbox_id].used) {
        return 0;
    }

    // Освобождаем память санбокса
    kfree(sandboxes[sandbox_id].memory_ptr);

    sandboxes[sandbox_id].used = 0;
    sandbox_count--;

    printk("Sandbox destroyed: %d\n", sandbox_id);
    return 1;
}

int sandbox_grant_capability(int sandbox_id, uint32_t capability) {
    if (sandbox_id < 0 || sandbox_id >= 32 || !sandboxes[sandbox_id].used) {
        return 0;
    }

    sandboxes[sandbox_id].capabilities |= capability;
    return 1;
}

int sandbox_revoke_capability(int sandbox_id, uint32_t capability) {
    if (sandbox_id < 0 || sandbox_id >= 32 || !sandboxes[sandbox_id].used) {
        return 0;
    }

    sandboxes[sandbox_id].capabilities &= ~capability;
    return 1;
}

// Проверка прав доступа для санбокса
int sandbox_check_permission(int sandbox_id, uint32_t required_caps) {
    if (sandbox_id < 0 || sandbox_id >= 32 || !sandboxes[sandbox_id].used) {
        return 0;
    }
    
    return (sandboxes[sandbox_id].capabilities & required_caps) == required_caps;
}

// Изоляция памяти санбокса
void* sandbox_alloc(int sandbox_id, size_t size) {
    if (!sandbox_check_permission(sandbox_id, CAP_ACCESS_MEMORY)) {
        return 0;
    }
    
    // Выделяем память внутри санбокса
    void* ptr = kmalloc(size);
    if (ptr) {
        // TODO: Добавить в таблицу памяти санбокса
    }
    
    return ptr;
}

// Обработчик системных вызовов для санбоксов
int sandbox_syscall(int sandbox_id, int syscall_num, void* args) {
    (void)args; // Пока не используем args
    
    if (!sandbox_check_permission(sandbox_id, CAP_SYSCALL)) {
        return -1;
    }
    
    // TODO: Реализовать обработку системных вызовов в зависимости от прав санбокса
    switch (syscall_num) {
        case SYS_IPC_SEND:
            if (sandbox_check_permission(sandbox_id, CAP_IPC)) {
                // Разрешить отправку IPC
                return 0;
            }
            break;
        // Другие системные вызовы
        default:
            break;
    }
    
    return -1;
}
