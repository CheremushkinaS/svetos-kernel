#include <kernel/module.h>
#include <kernel/interfaces/generic.h>
#include <kernel/printk.h>
#include <kernel/hal/port_io.h>

#define ATA_DATA        0x1F0
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW     0x1F3
#define ATA_LBA_MID     0x1F4
#define ATA_LBA_HIGH    0x1F5
#define ATA_DRIVE_HEAD  0x1F6
#define ATA_COMMAND     0x1F7
#define ATA_STATUS      0x1F7

static int ata_wait_busy() {
    unsigned char status;
    int timeout = 100000;
    
    while (timeout--) {
        status = inb(ATA_STATUS);
        if (!(status & 0x80)) return 0; // Busy bit cleared
    }
    return -1; // Timeout
}

static int ata_wait_drq() {
    unsigned char status;
    int timeout = 100000;
    
    while (timeout--) {
        status = inb(ATA_STATUS);
        if (status & 0x08) return 0; // DRQ set
        if (status & 0x01) return -1; // Error
    }
    return -1; // Timeout
}

static int ata_read_sectors(void* device, uint64_t lba, void* buffer, uint32_t count) {
    (void)device; // Unused parameter
    
    if (ata_wait_busy() < 0) {
        printk("ATA: Device busy timeout\n");
        return -1;
    }
    
    // Send LBA and sector count
    outb(ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F)); // LBA mode, master drive
    outb(ATA_SECTOR_COUNT, count);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    
    // Send READ command
    outb(ATA_COMMAND, 0x20);
    
    uint16_t* buf = (uint16_t*)buffer;
    for (uint32_t sector = 0; sector < count; sector++) {
        if (ata_wait_drq() < 0) {
            printk("ATA: DRQ timeout on sector %d\n", sector);
            return -1;
        }
        
        // Read 256 words (512 bytes)
        for (int i = 0; i < 256; i++) {
            buf[i] = inw(ATA_DATA);
        }
        buf += 256;
    }
    
    printk("ATA: Read %d sectors from LBA %lld\n", count, lba);
    return 0;
}

static uint64_t ata_get_capacity(void* device) {
    (void)device; // Unused parameter
    // For demonstration - real implementation would detect actual capacity
    return 32 * 1024 * 1024; // 32MB
}

static int ata_ioctl(void* device, uint32_t cmd, void* arg) {
    (void)device; // Unused parameter
    (void)arg;    // Unused parameter
    
    printk("ATA: IOCTL command %d\n", cmd);
    return 0;
}

static struct storage_interface ata_storage = {
    .name = INTERFACE_BLOCK_STORAGE,
    .read = ata_read_sectors,
    .write = NULL, // TODO: Implement write
    .get_capacity = ata_get_capacity,
    .ioctl = ata_ioctl,
    .private_data = NULL
};

static void ata_init(void) {
    printk("ATA Disk Driver initialized as module\n");
    printk("  Provides: %s interface\n", INTERFACE_BLOCK_STORAGE);
    printk("  Capacity: %d MB\n", (int)(ata_get_capacity(NULL) / (1024 * 1024)));
}

static void ata_exit(void) {
    printk("ATA Disk Driver unloaded\n");
}

static void ata_register_interfaces(void) {
    interface_register(INTERFACE_BLOCK_STORAGE, &ata_storage);
}

// Структура модуля - должна быть глобальной
struct module ata_module = {
    .magic = MODULE_MAGIC,
    .name = "ata_disk_driver",
    .version = 0x100,
    .type = MODULE_TYPE_DRIVER,
    .register_interfaces = ata_register_interfaces,
    .init = ata_init,
    .exit = ata_exit
};
