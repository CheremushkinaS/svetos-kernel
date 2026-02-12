#include <kernel/initrd/initrd.h>
#include <kernel/elf.h>
#include <kernel/mm/mm.h>
#include <kernel/printk.h>
#include <kernel/string.h>
#include <kernel/mm/kmalloc.h>

void* load_elf_module(void* module_data, uint32_t size) {
    (void)size;

    elf_header_t* header = (elf_header_t*)module_data;

    if (header->magic != ELF_MAGIC) {
        printk("Invalid ELF magic: 0x%x\n", header->magic);
        return NULL;
    }

    if (header->type != 2) {
        printk("Not an executable ELF\n");
        return NULL;
    }

    if (header->machine != 3) {
        printk("Not i386 ELF\n");
        return NULL;
    }

    elf_program_header_t* ph = (elf_program_header_t*)((uint32_t)module_data + header->phoff);
    for (int i = 0; i < header->phnum; i++, ph++) {
        if (ph->type == PT_LOAD) {
            void* seg_addr = kmalloc(ph->memsz);
            if (!seg_addr) {
                printk("Failed to allocate memory for segment\n");
                return NULL;
            }

            memcpy(seg_addr, (void*)((uint32_t)module_data + ph->offset), ph->filesz);

            if (ph->memsz > ph->filesz) {
                memset((void*)((uint32_t)seg_addr + ph->filesz), 0, ph->memsz - ph->filesz);
            }
        }
    }

    return (void*)header->entry;
}

int load_module_from_cpio(const char* filename) {
    uint32_t size;
    void* module_data = cpio_get_file(filename, &size);

    if (!module_data) {
        printk("Module not found: %s\n", filename);
        return -1;
    }

    printk("Loading module %s, size: %d\n", filename, size);

    void* entry_point = load_elf_module(module_data, size);
    if (!entry_point) {
        printk("Failed to load ELF module %s\n", filename);
        return -1;
    }

    int (*module_init)() = (int (*)())entry_point;
    int result = module_init();

    printk("Module %s loaded at 0x%x, init returned %d\n", filename, entry_point, result);
    return result;
}

int load_module(void* module_data, const char* name) {
    void* entry_point = load_elf_module(module_data, 0);
    if (!entry_point) {
        printk("Failed to load module %s\n", name);
        return -1;
    }

    int (*module_init)() = (int (*)())entry_point;
    int result = module_init();

    printk("Module %s loaded at 0x%x, init returned %d\n", name, entry_point, result);
    return result;
}
