# Lab 009 — One ADT, Two Representations

## Build and test

    cmake -S . -B build
    cmake --build build
    ./build/ch009_stack_tests

## Part A — Differential tests

Read the randomized test that executes the same operation stream on:
- ArrayStack
- LinkedStack
- a simple reference array

Explain why this checks ADT-equivalent behavior.

## Part B — Trace monotonic stack

Input:

    [4,5,2,10,8]

Hand-trace stack indices and outputs, then run:

    ./build/ch009_stack_demo

## Part C — Prove Θ(n)

Count pushes and pops for n inputs and derive an aggregate upper bound.

## Part D — Benchmark

    ./build/ch009_stack_benchmark

Compare array vs linked push/pop trend.

Discuss:
- malloc/free
- locality
- geometric growth
- cache effects

Do not generalize one measured ratio universally.

## Part E — Replace recursion

Write iterative DFS pseudocode using an explicit Stack ADT and state exactly what each stack element represents.
