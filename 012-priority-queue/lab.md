# Lab 012 — Build a Priority Queue

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch012_priority_queue_tests

## Part A — Hand Heap

Push priorities:

    4, 10, 7, 20, 3, 15

Draw array after each sift-up.

Then pop max repeatedly and draw each sift-down.

## Part B — Randomized Differential Test

Test compares heap output with a simple reference collection that scans for current maximum priority.

For equal priority, test accepts whichever matching item the heap returns because stability is not part of contract.

Explain why this is essential.

## Part C — Break Heap

Locally make sift_down choose lower-priority child.

Run validator/tests and find a failing sequence.

## Part D — Benchmark

    ./build/ch012_priority_queue_benchmark

Compare:
- heap priority queue
- unsorted reference with linear pop-max

Study mixed push/pop workloads.

## Part E — Design

Design a stable priority queue by adding sequence number.
Write comparator rules before coding.
