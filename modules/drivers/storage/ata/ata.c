#include <kernel/modules/module.h>
#include <kernel/printk.h>

MODULE_INIT(ata_init) {
    printk("ATA driver initialized\n");
    return 0;
}

MODULE_EXIT(ata_exit) {
    printk("ATA driver exited\n");
}

IPC_HANDLER(ata_ipc_handler) {
    // Обработка команд ATA
}
