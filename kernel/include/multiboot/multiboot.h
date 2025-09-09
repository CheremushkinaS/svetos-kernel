#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

/* Multiboot header magic numbers */
#define MULTIBOOT_HEADER_MAGIC      0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002

/* Flags for Multiboot header */
#define MULTIBOOT_PAGE_ALIGN        0x00000001
#define MULTIBOOT_MEMORY_INFO       0x00000002
#define MULTIBOOT_VIDEO_MODE        0x00000004
#define MULTIBOOT_AOUT_KLUDGE       0x00010000

/* Flags for Multiboot info structure */
#define MULTIBOOT_INFO_MEMORY       0x00000001
#define MULTIBOOT_INFO_BOOTDEV      0x00000002
#define MULTIBOOT_INFO_CMDLINE      0x00000004
#define MULTIBOOT_INFO_MODULES      0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS    0x00000010
#define MULTIBOOT_INFO_ELF_SYMS     0x00000020
#define MULTIBOOT_INFO_MEM_MAP      0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO   0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE 0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER  0x00000200
#define MULTIBOOT_INFO_APM_TABLE    0x00000400
#define MULTIBOOT_INFO_VIDEO_INFO   0x00000800

#ifndef __ASSEMBLER__

/* Multiboot header structure */
struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
    uint32_t mode_type;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
};

/* Memory map entry structure */
struct multiboot_mmap_entry {
    uint32_t size;
    uint32_t addr_low;
    uint32_t addr_high;
    uint32_t len_low;
    uint32_t len_high;
    uint32_t type;
} __attribute__((packed));

/* Multiboot information structure */
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    union {
        struct {
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t reserved;
        } aout_syms;
        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } elf_sec;
    } u;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
};

/* Module structure */
struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t cmdline;
    uint32_t reserved;
};

/* Typedefs for compatibility */
typedef struct multiboot_info multiboot_info_t;
typedef struct multiboot_module multiboot_module_t;
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

#endif /* __ASSEMBLER__ */

#endif /* MULTIBOOT_H */
