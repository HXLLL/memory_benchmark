#include "test.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void test_random_access(uint64_t step, uint64_t iter, uint64_t size) {
    uint64_t cycle = iteration(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}
void test_branch_prediction(uint64_t step, uint64_t iter, uint64_t size) {
    uint64_t cycle = branch_prediction(step, iter, size);
    printf("branch prediction test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}
void test_pointer_chasing(uint64_t step, uint64_t iter, uint64_t size) {
    uint64_t cycle = pointer_chasing(step, iter, size);
    printf("random access test, step=%lu, size=%lu, cycles: %lu, CPE: %.3lf\n", step, size, cycle, 1.0*cycle/iter);
}

int main() {
    int iter = 1e8, step = 1e5+7;
    size_t l1_test_size=K(8), l2_test_size=K(256),l3_test_size=M(4), dram_test_size=G(1);

    // l1 random access
    printf("l1 cache random access\n");
    test_random_access(step, iter, l1_test_size);
    printf("\n");

    // l2 random access
    printf("l2 cache random access\n");
    test_random_access(step, iter, l2_test_size);
    printf("\n");

    // l3 random access
    printf("l3 cache random access\n");
    test_random_access(step, iter, l3_test_size);
    printf("\n");

    // // dram random access
    // printf("dram random access\n");
    // test_random_access(step, iter, dram_test_size);
    // printf("\n");


    // l1 branch prediction
    printf("l1 cache branch prediction\n");
    test_branch_prediction(step, iter, l1_test_size);
    printf("\n");

    // l2 branch prediction
    printf("l2 cache branch prediction\n");
    test_branch_prediction(step, iter, l2_test_size);
    printf("\n");

    // l3 branch prediction
    printf("l3 cache branch prediction\n");
    test_branch_prediction(step, iter, l3_test_size);
    printf("\n");

    // // dram branch prediction
    // printf("dram branch prediction\n");
    // test_branch_prediction(step, iter, dram_test_size);
    // printf("\n");


    // l1 pointer chasing
    printf("l1 cache pointer chasing\n");
    test_pointer_chasing(step, iter, l1_test_size);
    printf("\n");

    // l2 pointer chasing
    printf("l2 cache pointer chasing\n");
    test_pointer_chasing(step, iter, l2_test_size);
    printf("\n");

    // l3 pointer chasing
    printf("l3 cache pointer chasing\n");
    test_pointer_chasing(step, iter, l3_test_size);
    printf("\n");

    // // dram pointer chasing
    // printf("dram pointer chasing\n");
    // test_pointer_chasing(step, iter, dram_test_size);
    // printf("\n");

}