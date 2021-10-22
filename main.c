#include "test.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// #define PROFILE

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

void seq_test_step_1() {
#ifdef PROFILE
    ProfilerStart("seq_test.prof");
#endif
    uint64_t step=1, iter = 1e8, size = 1U<<30;
    uint64_t cycle = iteration(step, iter, size);
    printf("sequential access, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void random_access_test() {
#ifdef PROFILE
    ProfilerStart("random_access.prof");
#endif
    uint64_t step=100007, iter = 1e8, size = 1U<<30;
    uint64_t cycle = iteration(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void seq_pointer_chasing() {
#ifdef PROFILE
    ProfilerStart("random_pointer_chasing.prof");
#endif
    uint64_t step=1, iter = 1e8, size = 1U<<30;
    uint64_t cycle = pointer_chasing(step, iter, size);
    printf("random pointer chasing test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void random_pointer_chasing() {
#ifdef PROFILE
    ProfilerStart("random_pointer_chasing.prof");
#endif
    uint64_t step=1e9+8, iter = 1e8, size = 1U<<30;
    uint64_t cycle = pointer_chasing(step, iter, size);
    printf("random pointer chasing test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
#ifdef PROFILE
    ProfilerStop();
#endif
}
void l3_random_access() {
    uint64_t step=100007, iter = 1e8, size = 1<<20<<2;
    uint64_t cycle = iteration(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}
void test_random_access(uint64_t step, uint64_t iter, uint64_t size) {
    uint64_t cycle = iteration(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}
void test_pointer_chasing(uint64_t step, uint64_t iter, uint64_t size) {
    uint64_t cycle = pointer_chasing(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}

int main() {
    // random_pointer_chasing();

    // l1 random access, 8KB
    printf("l1 cache random access\n");
    test_random_access(100007, 1e8, 1<<10<<3);
    printf("\n");

    // l2 random access, 256KB
    printf("l2 cache random access\n");
    test_random_access(100007, 1e8, 1<<10<<8);
    printf("\n");

    // l3 random access, 2MB
    printf("l3 cache random access\n");
    test_random_access(100007, 1e8, 1<<20<<2);
    printf("\n");

    // dram random access, 1GB
    printf("dram random access\n");
    test_random_access(100007, 1e8, 1<<30);
    printf("\n");

    // l1 pointer chasing, 8KB
    printf("l1 cache pointer chasing\n");
    test_pointer_chasing(100007, 1e8, 1<<10<<3);
    printf("\n");

    // l2 pointer chasing, 256KB
    printf("l2 cache pointer chasing\n");
    test_pointer_chasing(100007, 1e8, 1<<10<<8);
    printf("\n");

    // l3 pointer chasing, 2MB
    printf("l3 cache pointer chasing\n");
    test_pointer_chasing(100007, 1e8, 1<<20<<2);
    printf("\n");

    // dram pointer chasing, 1GB
    printf("dram pointer chasing\n");
    test_pointer_chasing(100007, 1e8, 1<<30);
    printf("\n");

}