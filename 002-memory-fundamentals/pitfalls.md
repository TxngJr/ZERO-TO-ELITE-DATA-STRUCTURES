# Pitfalls — Chapter 002

## Use-after-free
Address bits may remain visible after free; that does not make dereference valid.

## Lost realloc pointer
Do not overwrite the only pointer before checking realloc success.

## Out-of-bounds
Array syntax does not perform automatic bounds checking in C.

## Double free
Ownership ambiguity often causes multiple components to believe they should release the same allocation.

## Stack/heap mythology
Do not assume every local variable physically sits on a runtime stack or that "heap" is one simple contiguous area.

## Benchmark overclaim
One faster run does not establish a universal rule. Measure repeatedly and control variables.
