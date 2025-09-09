#include <kernel/mm/mm.h>
#include <kernel/debug.h>
#include <kernel/printk.h>

void test_basic_allocation() {
    debug_test_start("Basic Allocation");
    void* ptr = kmalloc(32);
    ASSERT(ptr != NULL, "Allocate 32 bytes");
    kfree(ptr);
    debug_test_end();
}

void test_oversized_allocation() {
    debug_test_start("Oversized Allocation");
    void* large = kmalloc(1024 * 1024 * 1024); // 1 GB
    ASSERT(large == NULL, "Oversized allocation should fail");
    debug_test_end();
}

void test_multiple_allocations() {
    debug_test_start("Multiple Allocations");
    void* blocks[10];
    for (int i = 0; i < 10; i++) {
        blocks[i] = kmalloc(32);
        ASSERT(blocks[i] != NULL, "Allocation #%d", i);
    }
    for (int i = 0; i < 10; i++) {
        kfree(blocks[i]);
    }
    debug_test_end();
}

void test_fragmentation() {
    debug_test_start("Fragmentation");
    void* block1 = kmalloc(128);
    void* block2 = kmalloc(128);
    kfree(block1);
    void* new_block = kmalloc(256); // Должен использовать освобожденное место
    ASSERT(new_block != NULL, "Allocation after partial free");
    kfree(block2);
    kfree(new_block);
    debug_test_end();
}

void test_mixed_sizes() {
    debug_test_start("Mixed Sizes");
    void* p1 = kmalloc(8);
    void* p2 = kmalloc(16);
    void* p3 = kmalloc(32);
    ASSERT(p1 != NULL && p2 != NULL && p3 != NULL, "Allocate mixed sizes");
    kfree(p2);
    void* p4 = kmalloc(16); // Должен войти в освобожденное место
    ASSERT(p4 != NULL, "Allocation in freed space");
    kfree(p1);
    kfree(p3);
    kfree(p4);
    debug_test_end();
}

void mm_run_all_tests() {
    printk("Running memory manager tests...\n");
    test_basic_allocation();
    test_oversized_allocation();
    test_multiple_allocations();
    test_fragmentation();
    test_mixed_sizes();
    printk("Memory tests completed.\n");
}
