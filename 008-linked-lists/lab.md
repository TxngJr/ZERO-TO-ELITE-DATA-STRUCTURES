# Lab 008 — Follow the Pointers

## Build and test

~~~bash
cmake -S . -B build
cmake --build build
./build/ch008_list_tests
~~~

## Part A — GDB

~~~bash
gdb ./build/ch008_list_demo
~~~

Break after three pushes and inspect:
- list metadata
- node addresses
- next links

Because types are opaque, you may set breakpoint inside implementation to inspect internals.

## Part B — Draw mutations

For singly list:
1. [10,20,30]
2. insert 15 at index 1
3. erase index 2
4. pop_back

Draw head/tail after every step.

## Part C — Break invariant intentionally

In a local uncommitted edit, skip updating tail when erasing last element.
Run tests and explain which validation detects it.

## Part D — Circular reasoning

Create [1,2,3], rotate left twice.
Predict logical order after each rotation.

## Part E — Benchmark

~~~bash
./build/ch008_list_vector_benchmark
~~~

Compare traversal of:
- IntVector
- IntSList

Both Θ(n), but explain practical difference using locality/allocation metadata.

Do not turn one run into a universal speed ratio.
