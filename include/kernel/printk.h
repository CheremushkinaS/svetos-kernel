#pragma once

#include <stddef.h>
#include <stdint.h>  // Гарантируем наличие uintX_t

void printk(const char* format, ...);
void early_printk(const char* message);
