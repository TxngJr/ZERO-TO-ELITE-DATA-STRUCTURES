# Implementation Notes — Chapter 002

## memory_walk.c

Purpose:
- show addresses without claiming fixed layout
- allocate one heap object
- verify allocation failure handling
- free owned memory exactly once
- show struct sizeof as an observable ABI result

## locality.c

Purpose:
- allocate a row-major matrix
- traverse rows then columns
- keep sums observable so loops cannot be trivially removed
- compare repeated timing trends

It is an educational microbenchmark, not a production-quality benchmarking harness. Chapter 142 will cover warm-up, CPU frequency, dead-code elimination, variance and percentiles.
