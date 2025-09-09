#include <hal/serial.h>
#include <hal/port_io.h>

void serial_configure_baud_rate(uint16_t com, uint16_t divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(uint16_t com) {
    // 8 бит данных, нет паритета, 1 стоп-бит
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_fifo(uint16_t com) {
    // Включить FIFO, очистить их, 14 байт размера
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(uint16_t com) {
    // Установить RTS и DTR
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(uint16_t com) {
    // Проверить бит 5 регистра статуса линии
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write_char(uint16_t com, char c) {
    while (!serial_is_transmit_fifo_empty(com));
    outb(SERIAL_DATA_PORT(com), c);
}

void serial_write_string(uint16_t com, const char* str) {
    while (*str) {
        serial_write_char(com, *str);
        str++;
    }
}

void serial_init(uint16_t com, uint16_t divisor) {
    serial_configure_baud_rate(com, divisor);
    serial_configure_line(com);
    serial_configure_fifo(com);
    serial_configure_modem(com);
}

// Новые функции для использования в printk
void serial_init_com1(void) {
    serial_init(SERIAL_COM1_BASE, 1); // 115200 baud
}

int serial_is_ready(void) {
    return serial_is_transmit_fifo_empty(SERIAL_COM1_BASE);
}

void serial_putc(char c) {
    serial_write_char(SERIAL_COM1_BASE, c);
}
