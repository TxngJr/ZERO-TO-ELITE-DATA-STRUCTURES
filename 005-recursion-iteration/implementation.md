# Implementation Notes — Chapter 005

src/recursion_iteration.c implements paired functions:

- factorial_recursive / factorial_iterative
- sum_recursive / sum_iterative
- gcd_recursive / gcd_iterative
- binary_search_recursive / binary_search_iterative

The tests use the iterative and recursive versions as mutual differential checks over many inputs.

## Binary-search interval convention

Both implementations use half-open interval:

    [lo, hi)

Benefits:
- length = hi - lo
- empty when lo == hi
- right half starts at mid+1
- fewer off-by-one special cases

## Preconditions

factorial:
    n <= 20 for uint64_t result in this teaching API

binary search:
    data[0..n) sorted ascending

Violating a precondition means the caller is outside the documented contract; Chapter 141 will expand testing strategy for contracts.

## No tail-call assumption

The source includes ordinary recursion but does not claim tail-call optimization or constant stack.
