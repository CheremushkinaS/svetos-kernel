#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(expr, msg, ...) \
    do { \
        if (!(expr)) { \
            debug_assert(0, __FILE__, __LINE__, msg, ##__VA_ARGS__); \
        } \
    } while (0)

void debug_set_test_name(const char* name);
void debug_test_start(const char* name);
void debug_test_end();
void debug_assert(int condition, const char* file, int line, const char* format, ...);
void panic(const char *message, const char *file, uint32_t line);

#endif
