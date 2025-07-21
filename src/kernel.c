#include <stdint.h>
#include <kernel/printk.h>
#include <kernel/mm/mm_api.h>

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
};

void init_video() {
    volatile char *video = (volatile char*)0xB8000;
    const char *str = "SvetOS Kernel Loaded!";
    for(int i = 0; str[i]; i++) {
        video[i*2] = str[i];
        video[i*2+1] = 0x0F;
    }
}

void mm_test_basic() {
    printk("\n=== Memory Manager Tests ===\n");
    
    // Тест 1: Базовые аллокации
    void *ptr1 = kalloc(128);
    printk("Test 1: Allocated 128B at 0x%x\n", ptr1);
    
    void *ptr2 = kalloc(2048);
    printk("Test 1: Allocated 2048B at 0x%x\n", ptr2);
    
    kfree(ptr1);
    printk("Test 1: Freed 128B at 0x%x\n", ptr1);
    
    // Тест 2: Проверка записи
    int *test_val = (int*)kalloc(64);
    *test_val = 0xCAFEBABE;
    printk("Test 2: Wrote 0x%x to 0x%x\n", *test_val, test_val);
    kfree(test_val);
    
    // Тест 3: Множественные аллокации
    void *pages[5];
    for(int i = 0; i < 5; i++) {
        pages[i] = kalloc(4096);
        printk("Test 3: Allocated page %d at 0x%x\n", i, pages[i]);
    }
    
    for(int i = 0; i < 5; i++) {
        kfree(pages[i]);
        printk("Test 3: Freed page %d at 0x%x\n", i, pages[i]);
    }
}

void mm_test_advanced() {
    printk("\n=== Advanced Memory Tests ===\n");
    
    // Тест 4: Фрагментация
    void *small1 = kalloc(512);
    void *large = kalloc(8192);
    void *small2 = kalloc(512);
    
    printk("Test 4: Allocated small1:0x%x, large:0x%x, small2:0x%x\n", 
           small1, large, small2);
    
    kfree(small1);
    void *medium = kalloc(1024);
    printk("Test 4: Allocated medium:0x%x (should be near small1)\n", medium);
    
    kfree(medium);
    kfree(large);
    kfree(small2);
    
    // Тест 5: Граничные случаи
    void *zero = kalloc(0);
    printk("Test 5: Alloc 0 bytes: 0x%x\n", zero);
    
    void *huge = kalloc(1024 * 1024 * 16); // 16MB
    printk("Test 5: Alloc huge block: 0x%x\n", huge);
    if(huge) kfree(huge);
}

void kmain(uint32_t magic, struct multiboot_info *mb_info) {
    (void)magic;
    (void)mb_info;
    
    init_video();
    early_printk("Starting SvetOS kernel...");
    
    // Инициализация памяти
    mm_init();
    
    // Запуск тестов
    mm_test_basic();
    mm_test_advanced();
    
    // Вечный цикл
    printk("\nAll tests completed! System halted.");
    while(1) {
        asm volatile ("hlt");
    }
}
