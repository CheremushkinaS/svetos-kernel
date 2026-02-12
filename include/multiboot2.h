#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

/* Multiboot2 types and constants */
typedef unsigned char multiboot_uint8_t;
typedef unsigned short multiboot_uint16_t;
typedef unsigned int multiboot_uint32_t;
typedef unsigned long long multiboot_uint64_t;

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_MODULE 3

struct multiboot_tag {
    multiboot_uint32_t type;
    multiboot_uint32_t size;
};

struct multiboot_tag_module {
    multiboot_uint32_t type;
    multiboot_uint32_t size;
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;
    multiboot_uint32_t cmdline;
};

#endif
