#include "test.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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