#include "recursion_iteration.h"

#include <stdio.h>

int main(void) {
    const int data[] = {1, 3, 5, 7, 9, 11, 13};

    printf("5! recursive=%llu iterative=%llu\n",
           (unsigned long long)factorial_recursive(5),
           (unsigned long long)factorial_iterative(5));

    printf("gcd(48,18) recursive=%u iterative=%u\n",
           gcd_recursive(48, 18),
           gcd_iterative(48, 18));

    printf("search 7 recursive=%td iterative=%td\n",
           binary_search_recursive(data, 7, 7),
           binary_search_iterative(data, 7, 7));

    return 0;
}
