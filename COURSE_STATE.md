# Course State

Current Batch: 02

Completed Chapters:
- 001 Programming Foundations
- 002 Memory Fundamentals
- 003 Abstract Data Type (ADT)
- 004 Complexity Analysis
- 005 Recursion & Iteration
- 006 Arrays

Current Chapter: 006

Next Chapter: 007 Strings

Concepts Covered:
- variables/functions/loops/pointers/references/generics
- stack/heap/address/allocation/locality
- ADT/interface/representation/invariant
- O, Ω, Θ, little-o, little-ω
- best/average/worst/amortized analysis
- aggregate/accounting/potential method preview
- recursion/call stack/base case/progress
- induction and loop invariant correspondence
- recursion vs iteration / binary search
- static arrays / dynamic arrays / multidimensional arrays
- size vs capacity / geometric growth
- array-to-pointer decay / one-past ranges
- pointer invalidation / row-major layout

Structures Implemented:
- opaque C IntStack
- opaque C IntVector with reserve, push/pop, get/set, insert/erase, clear, shrink-to-fit

Tests Added:
- Batch 01 smoke/unit tests
- complexity operation-count tests
- recursive/iterative differential tests
- IntVector unit tests
- deterministic 10,000-step randomized differential vector test

Benchmarks Added:
- row-major vs column-major locality
- linear vs quadratic scaling
- recursion vs iteration teaching benchmark
- vector push-back vs front-insert scaling

Known Dependencies:
- Chapter 007 Strings builds on contiguous character arrays and dynamic storage
- Chapter 008 Linked Lists contrasts pointer chasing with arrays
- Chapter 009 Stack revisits ADT Stack using array and linked representations

Open Problems:
- none if CI passes for Batch 02

Cross References:
- 004 amortized analysis → 006 vector growth → 131 amortized structures
- 005 call stack → 009 Stack ADT distinction → tree traversals later
- 002 locality → 006 contiguous arrays → 008 linked-list contrast
- 003 ADT → 006 IntVector API/invariants

Coverage: 6 / 170 chapters
