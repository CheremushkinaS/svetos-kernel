#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Ports для COM1
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

// Конфигурация последовательного порта
#define SERIAL_LINE_ENABLE_DLAB 0x80

void serial_configure_baud_rate(uint16_t com, uint16_t divisor);
void serial_configure_line(uint16_t com);
void serial_configure_fifo(uint16_t com);
void serial_configure_modem(uint16_t com);
int serial_is_transmit_fifo_empty(uint16_t com);
void serial_write_char(uint16_t com, char c);
void serial_write_string(uint16_t com, const char* str);
void serial_init(uint16_t com, uint16_t divisor);

// Новые функции для использования в printk
void serial_init_com1(void);
int serial_is_ready(void);
void serial_putc(char c);

#endif // SERIAL_H
