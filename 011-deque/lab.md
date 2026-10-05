# Lab 011 — Both Ends and a Monotonic Window

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch011_deque_tests

## Part A — Ring trace

Start empty:
1. push_back 10
2. push_front 5
3. push_back 20
4. pop_front
5. push_front 1
6. pop_back

Draw head, size and physical slots after every operation.

## Part B — Randomized differential test

Compare IntDeque against a reference array where front/back operations are simulated directly.

Explain why the reference may be slower but still valuable for correctness testing.

## Part C — Sliding maximum

Trace:

    [1,3,-1,-3,5,3,6,7], k=3

Write deque indices and values each step.

## Part D — Aggregate proof

Count:
- total pushes
- total front pops
- total back pops

Derive O(n).

## Part E — Benchmark

    ./build/ch011_deque_benchmark

Compare alternating operations that force wrap-around against a reference shifting model.
