#include <kernel/hal/serial.h>
#include <kernel/hal/port_io.h>
#include <kernel/printk.h>

void serial_init(void) {
    // Настройка последовательного порта COM1
    outb(0x3F8 + 1, 0x00); // Отключить прерывания
    outb(0x3F8 + 3, 0x80); // Включить DLAB
    outb(0x3F8 + 0, 0x03); // Установить делитель 3 (38400 бод)
    outb(0x3F8 + 1, 0x00); // Старший байт делителя
    outb(0x3F8 + 3, 0x03); // 8 бит, нет паритета, 1 стоп бит
    outb(0x3F8 + 2, 0xC7); // Включить FIFO
    outb(0x3F8 + 4, 0x0B); // Включить IRQ
}

// Альтернативное имя для совместимости
void serial_initialize(void) {
    serial_init();
}

void serial_init_com1(void) {
    serial_init();
}

int serial_is_ready(void) {
    // Проверяем, готов ли порт к передаче (бит 5 установлен)
    return (inb(0x3F8 + 5) & 0x20) != 0;
}

void serial_write(char c) {
    // Ждем, пока буфер передачи не станет пустым
    while (!serial_is_ready());
    outb(0x3F8, c);
}

void serial_putc(char c) {
    if (c == '\n') {
        serial_write('\r');
    }
    serial_write(c);
}

void serial_puts(const char* str) {
    while (*str) {
        serial_putc(*str++);
    }
}

int serial_received(void) {
    return inb(0x3F8 + 5) & 1;
}

char serial_read(void) {
    while (!serial_received());
    return inb(0x3F8);
}
