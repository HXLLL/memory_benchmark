#ifndef TEST_H
#define TEST_H

// #include <gperftools/profiler.h>
#include <stddef.h>
#include <stdint.h>

#define ENABLE_PROFILE 0

#define K(n) (((uint64_t)n) << 10)
#define M(n) (((uint64_t)n) << 20)
#define G(n) (((uint64_t)n) << 30)

#define PROFILE(func, filename)                                                \
    do {                                                                       \
        if (ENABLE_PROFILE) ProfilerStart(filename);                           \
        func;                                                                  \
        if (ENABLE_PROFILE) ProfilerStop();                                    \
    } while (0)

#endif

/**
 * @brief Pointer chasing benchmark, used to measure memory latency. (Actually not
 * real pointer chasing) First fill the array with random element. Then in each
 * iteration, do something like pos += step + a[pos], to make sure memory access
 * on the critical path
 *
 * @param step
 * @param ite number of iteration
 * @param size size of the array, change it to control the level of cache to use
 * @return uint64_t total cycles
 */
uint64_t pointer_chasing(uint64_t step, uint64_t iter, size_t size);

/**
 * @brief Iteration benchmark, used to determine memory throughput.
 * On each iteration, do something like sum += a[pos], pos += step
 *
 * @param step
 * @param iter number of iteration
 * @param size size of the array, change it to control the level of cache to use
 * @return uint64_t total cycles
 */
uint64_t iteration(uint64_t step, uint64_t iter, size_t size);

/**
 * @brief branch prediction number, used to measure the performance degregation caused by
 * failed branch prediction. On every iteration, inc *sum* if a[pos]%2==1
 * 
 * @param step
 * @param iter number of iteration
 * @param size size of the array, change it to control the level of cache to use
 * @return uint64_t total cycles
 */
uint64_t branch_prediction(uint64_t step, uint64_t iter, size_t size);
