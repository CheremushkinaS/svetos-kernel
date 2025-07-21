#include "mm_types.h"
#include "mm_api.h"
#include "../ipc.h"

typedef struct {
    uintptr_t phys_base;
    size_t size;
    module_id_t owner;
    module_id_t consumer;
} ipc_buffer_t;

ipc_buffer_t *create_shared_buffer(module_id_t src, module_id_t dst, size_t size) {
    ipc_buffer_t *buf = kmalloc(sizeof(ipc_buffer_t));
    buf->phys_base = (uintptr_t)alloc_pages(size_to_order(size));
    buf->size = size;
    buf->owner = src;
    buf->consumer = dst;
    
    // Отображаем в оба адресных пространства
    mm_space_t *src_space = get_module_space(src);
    mm_space_t *dst_space = get_module_space(dst);
    
    map_region(src_space, SHARED_BASE_SRC, buf->phys_base, size, MEM_READ | MEM_WRITE);
    map_region(dst_space, SHARED_BASE_DST, buf->phys_base, size, MEM_READ);
    
    return buf;
}
