#include <stdint.h>

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

void kmain(uint32_t magic, struct multiboot_info *mb_info) {
    init_video();
    while(1);
}
