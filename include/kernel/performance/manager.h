#ifndef _PERFORMANCE_MANAGER_H
#define _PERFORMANCE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_PERF_CALLBACKS 16
#define PERFORMANCE_ANALYSIS_INTERVAL 1000000000 // 1 second in ns

// Пороги для оптимизации
#define IPC_OPTIMIZATION_THRESHOLD 1000
#define MODULE_OPTIMIZATION_THRESHOLD 50
#define CONTEXT_SWITCH_THRESHOLD 10000
#define IPC_CACHE_HINT_THRESHOLD 0.7f
#define MODULE_PRELOAD_HINT_THRESHOLD 20
#define MEMORY_BATCH_HINT_THRESHOLD 100

typedef enum {
    PERF_STAT_IPC_CALLS,
    PERF_STAT_MODULE_LOADS,
    PERF_STAT_CONTEXT_SWITCHES,
    PERF_STAT_MEMORY_ALLOCS,
    PERF_STAT_CACHE_HITS,
    PERF_STAT_CACHE_MISSES
} perf_stat_type_t;

typedef enum {
    PERF_EVENT_IPC_OPTIMIZATION,
    PERF_EVENT_MODULE_OPTIMIZATION,
    PERF_EVENT_SCHEDULER_OPTIMIZATION,
    PERF_EVENT_MEMORY_OPTIMIZATION
} perf_event_type_t;

typedef struct {
    uint64_t ipc_calls;
    uint64_t module_loads;
    uint64_t context_switches;
    uint64_t memory_allocations;
    uint64_t cache_hits;
    uint64_t cache_misses;
    uint64_t last_analysis_time;
} performance_stats_t;

typedef struct {
    bool increase_ipc_cache;
    int ipc_cache_size_hint;
    bool preload_modules;
    int preload_hint_count;
    bool use_batch_alloc;
} optimization_hints_t;

typedef void (*perf_callback_t)(perf_event_type_t event_type, void* data);

void performance_manager_init(void);
int register_performance_callback(perf_callback_t callback);
void notify_performance_event(perf_event_type_t event_type, void* data);
void analyze_performance(void);
void update_performance_stats(perf_stat_type_t stat_type, uint64_t value);
optimization_hints_t get_optimization_hints(void);

#ifdef __x86_64__
void init_performance_counters(void);
#endif

#endif
