# Course State

Current Batch: 03

Completed Chapters:
- 001 Programming Foundations
- 002 Memory Fundamentals
- 003 Abstract Data Type (ADT)
- 004 Complexity Analysis
- 005 Recursion & Iteration
- 006 Arrays
- 007 Strings
- 008 Linked Lists
- 009 Stack

Current Chapter: 009

Next Chapter: 010 Queue

Concepts Covered:
- programming/memory/ADT foundations
- asymptotic and amortized analysis
- recursion, iteration and correctness invariants
- static/dynamic/multidimensional arrays
- byte strings, C strings, mutable/immutable strings
- null termination, length/capacity, encoding caveats
- singly/doubly/circular linked lists
- pointer chasing, stable-node addresses, sentinels/intrusive-list preview
- Stack ADT with array-backed and linked-backed representations
- monotonic stack and Next Greater Element
- aggregate proof that monotonic-stack processing is Θ(n)

Structures Implemented:
- opaque IntStack
- IntVector
- ByteString
- IntSList
- IntDList
- IntCList
- ArrayStack
- LinkedStack
- monotonic Next Greater index stack

Tests Added:
- all Batch 01–02 tests
- ByteString deterministic and randomized mutation tests
- self-alias append/insert tests
- linked-list invariant validation
- 5,000-step randomized list differential tests
- circular-list rotation tests
- 10,000-step ArrayStack vs LinkedStack differential tests
- monotonic-stack correctness tests

Benchmarks Added:
- previous locality/scaling/vector benchmarks
- dynamic string builder vs repeated-copy construction
- direct vector vs direct singly-list Θ(n) traversal
- array stack vs linked stack push/pop roundtrip

Known Dependencies:
- Chapter 010 Queue builds on linked/array endpoint reasoning
- Chapter 011 Deque extends two-ended operations
- Chapter 012 Priority Queue separates FIFO/LIFO from priority ordering

Open Problems:
- none if Batch 03 CI passes

Cross References:
- 006 Arrays → 007 dynamic strings
- 002 locality + 006 arrays → 008 pointer-chasing contrast
- 003 ADT + 004 amortization + 006 vector → 009 Stack representations
- 005 recursion → 009 explicit Stack ADT
- 009 monotonic stack → future interview/competitive-programming patterns

Coverage: 9 / 170 chapters
