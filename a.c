#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>
#include <string.h>
#include <stdio.h>

// #define PROFILE

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

uint64_t pointer_chasing(uint64_t step, uint64_t iter) {
    size_t size=1U<<30; // 1G
    uint8_t *arr=malloc(size);
    uint64_t sum = 0, pos = 0;

    uint64_t begin=_rdtsc();

    asm volatile (
            "f2:\n\t"                                   // do
            "add %4, %1\n\t"                            // pos += step;
            "and $0x3fffffff, %1\n\t"                   // pos = pos & mask;
            "add 0x0(%3, %1, 1), %1\n\t"                // sum += arr[pos];
            "sub $1, %2\n\t"                            // --iter;
            "jne f2"                                    // while (iter > 0)
            : "+r" (sum), "+r" (pos), "+r" (iter), "+r" (arr), "+r" (step));

    uint64_t cycles = _rdtsc()-begin;

    free(arr);

    return cycles;
}
uint64_t iteration(uint64_t step, uint64_t iter) {
    size_t size=1U<<30; // 1G
    uint8_t *arr=malloc(size);
    uint64_t sum = 0, pos = 0;

    uint64_t begin=_rdtsc();

    asm volatile (
            "f1:\n\t"                                   // do
            "add %4, %1\n\t"                            // pos += step;
            "and $0x3fffffff, %1\n\t"                   // pos = pos & mask;
            "add 0x0(%3, %1, 1), %0\n\t"                // sum += arr[pos];
            "sub $1, %2\n\t"                            // --iter;
            "jne f1"                                    // while (iter > 0)
            : "+r" (sum), "+r" (pos), "+r" (iter), "+r" (arr), "+r" (step));

    uint64_t cycles = _rdtsc()-begin;

    printf("cycles: %ld\n", cycles);
    free(arr);

    return cycles;
}

void seq_test_step_1() {
#ifdef PROFILE
    ProfilerStart("seq_test.prof");
#endif
    uint64_t step=1, size = 1e8;
    uint64_t cycle = iteration(step, size);
    printf("sequential access, step=%lu, cycles: %lu, CPE: %.3lf\n", step, cycle, 1.0*cycle/size);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void random_access_test() {
#ifdef PROFILE
    ProfilerStart("random_access.prof");
#endif
    uint64_t step=1e9+8, size = 1e8;
    uint64_t cycle = iteration(step, size);
    printf("random access test, step=%lu, cycles: %lu, CPE: %.3lf\n", step, cycle, 1.0*cycle/size);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void seq_pointer_chasing() {
#ifdef PROFILE
    ProfilerStart("random_pointer_chasing.prof");
#endif
    uint64_t step=1, size = 1e8;
    uint64_t cycle = pointer_chasing(step, size);
    printf("random pointer chasing test, step=%lu, cycles: %lu, CPE: %.3lf\n", step, cycle, 1.0*cycle/size);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void random_pointer_chasing() {
#ifdef PROFILE
    ProfilerStart("random_pointer_chasing.prof");
#endif
    uint64_t step=1e9+8, size = 1e8;
    uint64_t cycle = pointer_chasing(step, size);
    printf("random pointer chasing test, step=%lu, cycles: %lu, CPE: %.3lf\n", step, cycle, 1.0*cycle/size);
#ifdef PROFILE
    ProfilerStop();
#endif
}

int main() {
    random_pointer_chasing();
    random_access_test();
    seq_test_step_1();
    seq_pointer_chasing();
}
