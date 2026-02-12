#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    MODULE_TYPE_SYSTEM,
    MODULE_TYPE_DRIVER,
    MODULE_TYPE_SERVICE,
    MODULE_TYPE_TEST
} module_type_t;

typedef enum {
    DRIVER_SUBTYPE_NONE,
    DRIVER_SUBTYPE_INPUT,
    DRIVER_SUBTYPE_VIDEO,
    DRIVER_SUBTYPE_STORAGE,
    DRIVER_SUBTYPE_NETWORK,
    DRIVER_SUBTYPE_AUDIO,
    DRIVER_SUBTYPE_TIME
} driver_subtype_t;

typedef enum {
    MODULE_STATUS_UNLOADED,
    MODULE_STATUS_LOADED,
    MODULE_STATUS_ERROR
} module_status_t;

typedef int (*module_init_func)(void);
typedef void (*module_exit_func)(void);
typedef int (*module_reload_func)(void);

#endif
