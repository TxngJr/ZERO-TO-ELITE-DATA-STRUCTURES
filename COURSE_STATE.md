# Course State

Current Batch: 01

Completed Chapters:
- 001 Programming Foundations
- 002 Memory Fundamentals
- 003 Abstract Data Type (ADT)

Current Chapter: 003

Next Chapter: 004 Complexity Analysis

Concepts Covered:
- variables, primitive values, control flow, functions
- recursion preview
- pointers, references, structs/classes, templates/generics
- stack vs heap, addresses, allocation/deallocation
- memory layout, alignment preview, cache/locality preview
- ADT vs data structure
- interface, representation, invariant, implementation
- opaque C type and a stack ADT implementation

Structures Implemented:
- IntStack as an opaque C ADT backed by a growable contiguous buffer

Tests Added:
- CTest smoke tests
- IntStack unit tests including empty, LIFO, growth and repeated reuse

Benchmarks Added:
- row-major vs column-major locality experiment

Known Dependencies:
- Chapter 004 will formalize asymptotic analysis
- Chapter 005 will formalize recursion/call stack
- Chapter 006 will build arrays/dynamic arrays without treating realloc as magic

Open Problems:
- none for Batch 01

Cross References:
- Chapter 001 pointers → Chapter 002 addresses → Chapter 003 hidden representation
- Chapter 003 IntStack will be revisited in Chapter 009 Stack

Coverage: 3 / 170 chapters
