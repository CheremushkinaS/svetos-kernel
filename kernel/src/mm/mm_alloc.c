#include <kernel/mm/mm.h>
#include <kernel/printk.h>

// Реализация функций аллокации будет добавлена позже
void* kmalloc_impl(size_t size) {
    printk("kmalloc_impl(%zu) called\n", size);
    return NULL;
}

void kfree_impl(void* ptr) {
    printk("kfree_impl(%p) called\n", ptr);
}
