#include <kernel/modules/module.h>
#include <kernel/printk.h>

MODULE_INIT(initrd_fs_init) {
    printk("Initrd filesystem initialized\n");
    return 0;
}

MODULE_EXIT(initrd_fs_exit) {
    printk("Initrd filesystem exited\n");
}

IPC_HANDLER(initrd_fs_ipc_handler) {
    // Обработка файловых операций
}
