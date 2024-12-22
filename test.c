#include "test.h"

#include <fcntl.h>
#include <immintrin.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

struct hugemem {
    int fd;
    void *addr;
    size_t size;
};

#define HUGEPAGE_PATH "/dev/hugepages"
#define HUGEPAGE_SIZE (1LL << 30)

size_t align_size(size_t size) {
    return (size + HUGEPAGE_SIZE - 1) & ~(HUGEPAGE_SIZE - 1);
}

struct hugemem* alloc_hugemem(size_t size) {
    struct hugemem *mem = malloc(sizeof(struct hugemem));
    size = align_size(size);
    mem->size = size;

    mem->fd = open(HUGEPAGE_PATH "/hugepagefile", O_CREAT | O_RDWR);
    if (mem->fd == -1) {
        perror("open");
        goto free_hugemem;
    }

    mem->addr = mmap(NULL, size, PROT_READ | PROT_WRITE,
        MAP_SHARED, mem->fd, 0);
    if (mem->addr == MAP_FAILED) {
        perror("mmap");
        goto close_fd;
    }

    return mem;

close_fd:
    close(mem->fd);
free_hugemem:
    free(mem);
    return NULL;
}

void free_hugemem(struct hugemem *mem) {
    int ret = munmap(mem->addr, mem->size);
    if (ret == -1) {
        perror("munmap");
    }
    close(mem->fd);
    unlink(HUGEPAGE_PATH "/hugepagefile");
    free(mem);
}

uint64_t my_rand(int seed, int n) {return seed%7;}

uint64_t pointer_chasing(uint64_t step, uint64_t iter, size_t size) {
    size_t mask = (size - 1) & ~7;
    uint8_t *arr = malloc(size);
    uint64_t sum = 0, pos = 0;

    for (size_t i = 0; i != size; ++i) { // "pseudo random"
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
    size_t mask = (size - 1) & ~7;
    uint64_t sum = 0, pos = 0;

    struct hugemem *mem = alloc_hugemem(size);
    if (mem == NULL) {
        return -1;
    }

    uint8_t *arr = mem->addr;
    for (size_t i = 0; i != size; ++i) { // "pesudo random"
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

    free_hugemem(mem);

    return cycles;
}
uint64_t branch_prediction(uint64_t step, uint64_t iter, size_t size) {
    size_t mask = (size - 1) & ~7;
    uint8_t *arr = malloc(size);
    uint64_t sum = 0, pos = 0, tmp = 0;

    for (size_t i = 0; i != size; ++i) { // "pesudo random"
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