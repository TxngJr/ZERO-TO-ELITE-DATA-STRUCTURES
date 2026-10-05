# Course State

Current Batch: 04

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
- 010 Queue
- 011 Deque
- 012 Priority Queue

Current Chapter: 012

Next Chapter: 013 Hashing Fundamentals

Concepts Covered:
- programming, memory, ADT and asymptotic foundations
- recursion/iteration and correctness invariants
- arrays, strings and linked lists
- Stack ADT + monotonic stack
- Queue FIFO semantics
- linked queue and growing circular queue
- ring-buffer wrap-around and logical-vs-physical order
- Deque two-ended operations
- monotonic queue / sliding-window maximum
- Priority Queue ADT
- complete binary-tree array layout
- max-heap invariant
- sift-up / sift-down
- priority ties and stability contract
- scheduler/fairness/decrease-key previews

Structures Implemented:
- previous Batch 01–03 structures
- CircularQueue
- LinkedQueue
- IntDeque
- Sliding Window Maximum monotonic index deque
- MaxPriorityQueue backed by binary max-heap

Tests Added:
- all previous tests
- 12,000-step Queue randomized differential test
- circular wrap/growth FIFO tests
- 12,000-step Deque randomized differential test
- monotonic sliding-window tests including duplicates
- 15,000-step Priority Queue randomized differential test
- equal-priority non-stability-safe tests
- heap invariant validation

Benchmarks Added:
- previous benchmarks
- circular queue vs linked queue roundtrip
- ring deque vs intentionally shifting reference
- binary heap priority queue vs unsorted linear-pop-max reference

Known Dependencies:
- Chapter 013 formalizes hash functions, collisions and load factor
- Chapter 014 implements hash tables
- Chapter 015 builds Hash Set / Hash Map abstractions

Open Problems:
- none if Batch 04 CI passes

Cross References:
- 006 dynamic arrays → 010 circular queue growth
- 008 linked lists → 010 linked queue
- 009 monotonic stack → 011 monotonic queue
- 004 aggregate analysis → 011 sliding-window Θ(n)
- 006 array layout → 012 binary heap representation
- 012 Priority Queue → 025 Heap and graph algorithms later

Coverage: 12 / 170 chapters
