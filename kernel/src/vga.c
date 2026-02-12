#include <kernel/vga.h>
#include <kernel/printk.h>

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// Добавляем недостающую функцию vga_init
void vga_init(void) {
    printk("VGA initialized\n");
    // Здесь может быть дополнительная инициализация VGA при необходимости
}
