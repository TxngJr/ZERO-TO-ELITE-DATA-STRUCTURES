#ifndef RECURSION_ITERATION_H
#define RECURSION_ITERATION_H

#include <stddef.h>
#include <stdint.h>

uint64_t factorial_recursive(unsigned n);
uint64_t factorial_iterative(unsigned n);

long long sum_recursive(const int *data, size_t n);
long long sum_iterative(const int *data, size_t n);

unsigned gcd_recursive(unsigned a, unsigned b);
unsigned gcd_iterative(unsigned a, unsigned b);

ptrdiff_t binary_search_recursive(const int *data, size_t n, int target);
ptrdiff_t binary_search_iterative(const int *data, size_t n, int target);

#endif
