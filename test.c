#include "test.h"

#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t my_rand(int seed, int n) {return seed%7;}

uint64_t pointer_chasing(uint64_t step, uint64_t iter, size_t size) {
    size_t mask = size - 1;
    uint8_t *arr = malloc(size);
    uint64_t sum = 0, pos = 0;

    for (int i = 0; i != size; ++i) { // "pseudo random"
        arr[i] = my_rand(i, 256);
    }

    uint64_t begin = _rdtsc();

    asm volatile("f1_pointer_chasing:\n\t"    // do
                 "add %4, %1\n\t"             // pos += step;
                 "and %5, %1\n\t"             // pos = pos & mask;
                 "add 0x0(%3, %1, 1), %1\n\t" // pos += arr[pos];
                 "sub $1, %2\n\t"             // --iter;
                 "jne f1_pointer_chasing"     // while (iter > 0)
                 : "+r"(sum), "+r"(pos), "+r"(iter), "+r"(arr), "+r"(step),
                   "+r"(mask));

    uint64_t cycles = _rdtsc() - begin;

    free(arr);

    return cycles;
}
uint64_t iteration(uint64_t step, uint64_t iter, size_t size) {
    size_t mask = size - 1;
    uint8_t *arr = malloc(size);
    uint64_t sum = 0, pos = 0;

    for (int i = 0; i != size; ++i) { // "pesudo random"
        arr[i] = my_rand(i, 256);
    }

    uint64_t begin = _rdtsc();

    asm volatile("f1_iteration:\n\t"          // do
                 "add %4, %1\n\t"             // pos += step;
                 "and %5, %1\n\t"             // pos = pos & mask;
                 "add 0x0(%3, %1, 1), %0\n\t" // sum += arr[pos];
                 "sub $1, %2\n\t"             // --iter;
                 "jne f1_iteration"           // while (iter > 0)
                 : "+r"(sum), "+r"(pos), "+r"(iter), "+r"(arr), "+r"(step),
                   "+r"(mask));

    uint64_t cycles = _rdtsc() - begin;

    free(arr);

    return cycles;
}
uint64_t branch_prediction(uint64_t step, uint64_t iter, size_t size) {
    size_t mask = size - 1;
    uint8_t *arr = malloc(size);
    uint64_t sum = 0, pos = 0, tmp = 0;

    for (int i = 0; i != size; ++i) { // "pesudo random"
        arr[i] = my_rand(i, 256);
    }

    uint64_t begin = _rdtsc();

    asm volatile("f1_branch_prediction:\n\t"   // do
                 "add %4, %1\n\t"              // pos += step;
                 "and %5, %1\n\t"              // pos = pos & mask;
                 "mov (%3, %1, 1), %6\n\t"     // tmp = arr[pos]
                 "and $1, %6\n\t"              // tmp &= 1
                 "je f2_branch_prediction\n\t" // if tmp==0 goto f2
                 "add $1, %0\n\t"              // sum += 1
                 "f2_branch_prediction:\n\t"   // f2
                 "sub $1, %2\n\t"              // --iter;
                 "jne f1_branch_prediction"    // while (iter > 0)
                 : "+r"(sum), "+r"(pos), "+r"(iter), "+r"(arr), "+r"(step),
                   "+r"(mask), "+r"(tmp));

    uint64_t cycles = _rdtsc() - begin;

    free(arr);

    return cycles;
}