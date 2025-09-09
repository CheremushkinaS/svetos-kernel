#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <kernel/types.h>

// Заглушка для планировщика - будет реализована позже
typedef uint32_t spinlock_t;

static inline void spinlock_init(spinlock_t* lock) {
    *lock = 0;
}

static inline void spinlock_lock(spinlock_t* lock) {
    // Заглушка - в реальности будет атомарная операция
    while (*lock) {}
    *lock = 1;
}

static inline void spinlock_unlock(spinlock_t* lock) {
    *lock = 0;
}

#endif // SCHEDULER_H
