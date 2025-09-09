#ifndef MM_H
#define MM_H

#include <stdint.h>
#include <stddef.h>
#include <multiboot.h>

typedef struct memory_stats {
    uint32_t total_memory;
    uint32_t heap_size;
    uint32_t allocated;
    uint32_t alloc_count;
    uint32_t free_count;
} memory_stats_t;

extern uint8_t* heap_start;
extern uint8_t* heap_end;
extern memory_stats_t mm_stats;

void mm_init(multiboot_info_t* mb_info);
void* kmalloc(size_t size);
void kfree(void* ptr);
void mm_print_stats(void);
void mm_dump_blocks(void);
void mm_run_all_tests(void);

#endif
