# Lab 010 — Make FIFO Visible

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch010_queue_tests

## Part A — Wrap-around

Use capacity growth trace:
1. enqueue values
2. dequeue several
3. enqueue more
4. observe logical order after physical wrap

Draw physical indexes and logical sequence separately.

## Part B — Differential testing

Randomized tests run identical operations on:
- CircularQueue
- LinkedQueue
- simple reference array model

Explain why matching only size is insufficient; returned front/dequeue values must also match.

## Part C — Intentional bug

Locally remove:

    tail = NULL

when linked queue becomes empty.

Run invariant tests and explain failure.

## Part D — Benchmark

    ./build/ch010_queue_benchmark

Compare enqueue+dequeue roundtrips.

Discuss locality and per-node allocation.

## Part E — BFS paper exercise

Use queue to trace BFS on:

    A -> B,C
    B -> D
    C -> E

Write queue state after each vertex.
