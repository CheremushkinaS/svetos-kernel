#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <kernel/tty.h>
#include <kernel/hal/serial.h>
#include <kernel/gdt.h>
#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/isr.h>
#include <kernel/interrupts/irq.h>
#include <kernel/interrupts/pic.h>
#include <kernel/mm/mm.h>
#include <kernel/initrd/initrd.h>
#include <kernel/modules/manager.h>
#include <kernel/modules/loader.h>
#include <kernel/ipc.h>
#include <kernel/string.h>
#include <multiboot.h>
#include <stdint.h>

static void initialize_subsystems(multiboot_info_t* mb_info);
static void process_multiboot_modules(multiboot_info_t* mb_info);
static void load_modules(void);
static void kernel_panic(const char* message);
static void* phys_to_virt(uint32_t phys_addr);
static uint32_t find_initrd_in_memory(void);

void kernel_main(uint32_t magic, uint32_t mb_info_phys) {
    // Initialize serial first for debugging
    serial_init_com1();
    printk("=== SVETOS UNIVERSAL KERNEL STARTING ===\n");
    printk("Serial initialized\n");
    printk("Multiboot magic: 0x%x\n", magic);

    // Multiboot 1 magic number
    if (magic != 0x2BADB002) {
        printk("Expected magic: 0x2BADB002 (Multiboot 1)\n");
        kernel_panic("Invalid multiboot magic");
    }

    // Convert physical address to virtual pointer
    multiboot_info_t* mb_info = (multiboot_info_t*)phys_to_virt(mb_info_phys);
    printk("Multiboot info structure at physical: 0x%x, virtual: 0x%x\n", mb_info_phys, (uint32_t)mb_info);

    printk("Initializing terminal...\n");
    terminal_initialize();
    printk("Terminal initialized\n");

    printk("Initializing subsystems...\n");
    initialize_subsystems(mb_info);

    printk("Processing multiboot modules...\n");
    process_multiboot_modules(mb_info);

    printk("Loading modules...\n");
    load_modules();

    printk("=== KERNEL INITIALIZATION COMPLETE ===\n");
    printk("Entering main kernel loop...\n");

    while (1) {
        ipc_process_messages();
        for (volatile int i = 0; i < 1000000; i++);
        __asm__ volatile ("hlt;");
    }
}

static void load_modules(void) {
    printk("Initializing module manager...\n");
    module_manager_init();
    printk("Module manager initialized\n");

    // Load critical kernel modules
    printk("Loading critical modules...\n");
    if (module_load_critical() > 0) {
        printk("Critical modules loaded successfully\n");
    } else {
        printk("No critical modules found\n");
    }

    // Load additional modules from initrd
    printk("Loading modules from initrd...\n");
    if (load_modules_from_initrd() > 0) {
        printk("Modules from initrd loaded successfully\n");
    } else {
        printk("No modules found in initrd\n");
    }

    printk("Module loading completed\n");
}

static void initialize_subsystems(multiboot_info_t* mb_info) {
    printk("Initializing core subsystems...\n");

    printk("GDT init...\n");
    gdt_init();
    printk("GDT initialized\n");

    printk("IDT init...\n");
    idt_init();
    printk("IDT initialized\n");

    printk("ISR install...\n");
    isr_install();
    printk("ISRs installed\n");

    printk("PIC remap...\n");
    pic_remap(0x20, 0x28);
    printk("PIC remapped\n");

    printk("IRQ install...\n");
    irq_install();
    printk("IRQs installed\n");

    printk("Memory manager init...\n");
    mm_init(mb_info);
    printk("Memory manager initialized\n");

    printk("IPC init...\n");
    ipc_init();
    printk("IPC system initialized\n");

    printk("Enabling interrupts...\n");
    __asm__ volatile ("sti");
    printk("Interrupts enabled\n");
}

static void* phys_to_virt(uint32_t phys_addr) {
    // If using identity mapping, just return the address
    // If using higher half kernel, add offset (e.g., 0xC0000000)
    return (void*)(phys_addr); // Change this if needed
}

static uint32_t find_initrd_in_memory(void) {
    // Look for CPIO signature or other initrd signs
    for (uint32_t addr = 0x100000; addr < 0x400000; addr += 4) {
        if (*(uint32_t*)addr == 0x070701) {  // CPIO magic
            // Verify it's actually an archive
            if (*(uint32_t*)(addr + 4) == 0 && *(uint32_t*)(addr + 8) == 0) {
                return addr;
            }
        }
        // Also look for other signatures (tar, etc)
    }
    return 0;
}

static void process_multiboot_modules(multiboot_info_t* mb_info) {
    // Detailed multiboot structure info
    printk("Multiboot info structure at: 0x%x\n", (uint32_t)mb_info);
    printk("Multiboot flags: 0x%x\n", mb_info->flags);

    // Print all available flags for diagnostics
    printk("Available multiboot flags:\n");
    if (mb_info->flags & MULTIBOOT_INFO_MEMORY) printk("  MEMORY\n");
    if (mb_info->flags & MULTIBOOT_INFO_BOOTDEV) printk("  BOOTDEV\n");
    if (mb_info->flags & MULTIBOOT_INFO_CMDLINE) printk("  CMDLINE\n");
    if (mb_info->flags & MULTIBOOT_INFO_MODULES) printk("  MODULES\n");
    if (mb_info->flags & MULTIBOOT_INFO_AOUT_SYMS) printk("  AOUT_SYMS\n");
    if (mb_info->flags & MULTIBOOT_INFO_ELF_SYMS) printk("  ELF_SYMS\n");
    if (mb_info->flags & MULTIBOOT_INFO_MEM_MAP) printk("  MEM_MAP\n");
    if (mb_info->flags & MULTIBOOT_INFO_DRIVE_INFO) printk("  DRIVE_INFO\n");
    if (mb_info->flags & MULTIBOOT_INFO_CONFIG_TABLE) printk("  CONFIG_TABLE\n");
    if (mb_info->flags & MULTIBOOT_INFO_BOOT_LOADER) printk("  BOOT_LOADER\n");
    if (mb_info->flags & MULTIBOOT_INFO_APM_TABLE) printk("  APM_TABLE\n");

    // Check if modules information is available
    if (!(mb_info->flags & MULTIBOOT_INFO_MODULES)) {
        printk("No modules information available in multiboot flags\n");

        // Try to find initrd manually in memory
        printk("Searching for initrd manually in memory...\n");
        uint32_t initrd_addr = find_initrd_in_memory();
        if (initrd_addr != 0) {
            printk("Found possible CPIO archive at 0x%x\n", initrd_addr);
            initrd_init(initrd_addr, initrd_addr + 0x10000);
            initrd_list_files();
            return;
        }
        printk("No CPIO archive found in memory\n");
        return;
    }

    if (mb_info->mods_count == 0) {
        printk("No modules loaded by bootloader\n");
        return;
    }

    printk("Bootloader loaded %d module(s)\n", mb_info->mods_count);
    printk("Modules address: 0x%x\n", mb_info->mods_addr);

    // Verify modules address is valid
    if (mb_info->mods_addr == 0 || mb_info->mods_addr < 0x100000) {
        printk("Invalid modules address\n");
        return;
    }

    // Convert physical address of modules array to virtual
    multiboot_module_t* mods = (multiboot_module_t*)phys_to_virt(mb_info->mods_addr);

    // Process all modules loaded by bootloader
    for (uint32_t i = 0; i < mb_info->mods_count; i++) {
        // Convert module start and end addresses
        uint32_t mod_start = (uint32_t)phys_to_virt(mods[i].mod_start);
        uint32_t mod_end = (uint32_t)phys_to_virt(mods[i].mod_end);

        // Check if module addresses look plausible
        if (mod_start > 0x100000 && mod_end > mod_start) {
            printk("Module %d: start=0x%x, end=0x%x, size=%d bytes\n",
                   i, mod_start, mod_end, mod_end - mod_start);

            // Consider first module as initrd
            if (i == 0) {
                printk("Initializing initrd at 0x%x - 0x%x\n", mod_start, mod_end);

                initrd_init(mod_start, mod_end);
                printk("Initrd initialized\n");

                initrd_list_files();

                if (!initrd_verify_signatures()) {
                    kernel_panic("Module signature verification failed");
                } else {
                    printk("Initrd signatures verified successfully\n");
                }
            }
        } else {
            printk("Module %d has invalid addresses: start=0x%x, end=0x%x\n",
                   i, mods[i].mod_start, mods[i].mod_end);
        }
    }
}

static void kernel_panic(const char* message) {
    printk("KERNEL PANIC: %s\n", message);
    printk("System halted\n");

    __asm__ volatile ("cli");
    while (1) {
        __asm__ volatile ("hlt");
    }
}
