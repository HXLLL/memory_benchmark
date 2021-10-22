#ifndef TEST_H
#define TEST_H

#include <gperftools/profiler.h>
#include <stdint.h>

uint64_t pointer_chasing(uint64_t step, uint64_t iter, size_t size);
uint64_t iteration(uint64_t step, uint64_t iter, size_t size);

#define ENABLE_PROFILE 0

#define PROFILE(func, filename)                                                \
    do {                                                                       \
        if (ENABLE_PROFILE) ProfilerStart(filename);                           \
        func;                                                                  \
        if (ENABLE_PROFILE) ProfilerStop();                                    \
    } while (0)

#endif