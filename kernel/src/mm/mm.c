#include <kernel/mm/mm.h>
#include <kernel/printk.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <multiboot.h>

#ifndef HEAP_SIZE
#define HEAP_SIZE (2 * 1024 * 1024) // 2MB heap by default
#endif

#define MIN_BLOCK_SIZE 16
#define ALIGNMENT 8
#define BLOCK_MAGIC 0xDEADBEEF
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

extern uint8_t _kernel_end;

uint8_t* heap_start = NULL;
uint8_t* heap_end = NULL;
memory_stats_t mm_stats;

typedef struct mem_block {
    size_t size;
    struct mem_block* next;
    bool free;
    uint32_t magic;
} mem_block_t;

static mem_block_t* first_block = NULL;

static void init_heap(multiboot_info_t* mb_info) {
    // Use multiboot info to determine available memory
    uint32_t available_memory = 0;

    if (mb_info->flags & MULTIBOOT_INFO_MEMORY) {
        available_memory = (mb_info->mem_lower + mb_info->mem_upper) * 1024;
        printk("Available memory: %u KB\n", mb_info->mem_lower + mb_info->mem_upper);
    } else {
        // Fallback if no memory info is available
        available_memory = 64 * 1024 * 1024; // Assume 64MB
        printk("No memory info available, assuming %u MB\n", available_memory / (1024 * 1024));
    }

    // Set heap start after kernel
    heap_start = (uint8_t*)(((uintptr_t)&_kernel_end + 4095) & ~4095);

    // Calculate heap size based on available memory
    uint32_t max_heap_size = available_memory - (uint32_t)heap_start;
    uint32_t actual_heap_size = HEAP_SIZE;

    if (actual_heap_size > max_heap_size) {
        actual_heap_size = max_heap_size;
        printk("Reducing heap size to %u KB due to memory constraints\n", actual_heap_size / 1024);
    }

    heap_end = heap_start + actual_heap_size;

    first_block = (mem_block_t*)heap_start;
    first_block->size = actual_heap_size - sizeof(mem_block_t);
    first_block->next = NULL;
    first_block->free = true;
    first_block->magic = BLOCK_MAGIC;

    mm_stats.heap_size = actual_heap_size;
    mm_stats.total_memory = available_memory;
    mm_stats.allocated = 0;
    mm_stats.alloc_count = 0;
    mm_stats.free_count = 0;

    printk("Heap: 0x%x - 0x%x (%u KB)\n", heap_start, heap_end, actual_heap_size/1024);
}

static bool is_valid_pointer(void* ptr) {
    if (!ptr || (uint8_t*)ptr < heap_start || (uint8_t*)ptr >= heap_end)
        return false;

    mem_block_t* block = (mem_block_t*)ptr - 1;
    return (block->magic == BLOCK_MAGIC);
}

void* kmalloc(size_t size) {
    if (!size) return NULL;
    if (!heap_start) return NULL; // Heap not initialized

    size = ALIGN(size);
    mem_block_t* current = first_block;
    mem_block_t* best_fit = NULL;

    while (current) {
        if (current->free && current->size >= size) {
            if (!best_fit || current->size < best_fit->size)
                best_fit = current;
        }
        current = current->next;
    }

    if (!best_fit) {
        printk("kmalloc(%zu) failed! Out of memory\n", size);
        return NULL;
    }

    size_t remaining = best_fit->size - size;
    if (remaining >= sizeof(mem_block_t) + MIN_BLOCK_SIZE) {
        mem_block_t* new_block = (mem_block_t*)((uint8_t*)(best_fit + 1) + size);
        new_block->size = remaining - sizeof(mem_block_t);
        new_block->free = true;
        new_block->next = best_fit->next;
        new_block->magic = BLOCK_MAGIC;

        best_fit->next = new_block;
        best_fit->size = size;
    }

    best_fit->free = false;
    mm_stats.allocated += best_fit->size + sizeof(mem_block_t);
    mm_stats.alloc_count++;

    return (void*)(best_fit + 1);
}

void kfree(void* ptr) {
    if (!ptr) return;

    if (!is_valid_pointer(ptr)) {
        printk("kfree: invalid pointer 0x%x\n", (uint32_t)ptr);
        return;
    }

    mem_block_t* block = (mem_block_t*)ptr - 1;
    block->free = true;
    mm_stats.allocated -= block->size + sizeof(mem_block_t);
    mm_stats.free_count++;

    mem_block_t* current = first_block;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += sizeof(mem_block_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void mm_init(multiboot_info_t* mb_info) {
    printk("Initializing memory manager\n");
    init_heap(mb_info);
}

void mm_print_stats(void) {
    uint32_t free_bytes = mm_stats.heap_size - mm_stats.allocated;
    printk("Memory stats:\n");
    printk("  Total: %u KB, Heap: %u KB, Free: %u KB\n",
           mm_stats.total_memory / 1024, mm_stats.heap_size / 1024, free_bytes / 1024);
    printk("  Allocations: %u, Frees: %u\n",
           mm_stats.alloc_count, mm_stats.free_count);
}

void mm_dump_blocks(void) {
    printk("Memory blocks:\n");
    mem_block_t* current = first_block;
    uint32_t i = 0;

    while (current) {
        printk("  #%d: 0x%x size=%zu %s\n",
               i++, (uint32_t)current, current->size,
               current->free ? "FREE" : "USED");
        current = current->next;
    }
}

int memory_module_init(void) {
    // Эта функция является точкой входа для модуля памяти
    // Раньше она использовалась для инициализации менеджера памяти,
    // но сейчас инициализация происходит напрямую в kernel_main через mm_init()

    // В текущей архитектуре эта функция не должна вызываться
    printk("WARNING: memory_module_init() is deprecated and should not be called\n");
    printk("Memory initialization is now handled by mm_init() in kernel_main\n");

    return 0; // Всегда возвращает 0 для совместимости
}
