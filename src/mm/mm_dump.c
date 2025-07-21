#include <kernel/mm/mm_api.h>
#include <kernel/printk.h>

void mm_init(void) {
    printk("Memory manager initialized\n");
}

void* kalloc(size_t size) {
    // Временная заглушка
    static uint32_t addr = 0x100000;
    printk("[kalloc] Allocated %u bytes at 0x%x\n", size, addr);
    return (void*)(addr += 0x1000);
}

void kfree(void* ptr) {
    printk("[kfree] Freed memory at 0x%x\n", ptr);
}

void mm_dump_map(void) {
    printk("Memory map dump not implemented yet\n");
}
