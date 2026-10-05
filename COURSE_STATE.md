# Course State

Current Batch: 05

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
- 013 Hashing Fundamentals
- 014 Hash Table
- 015 Hash Set / Hash Map

Current Chapter: 015

Next Chapter: 016 Trees Fundamentals

Concepts Covered:
- previous Chapters 001–012
- hash value vs bucket index vs equality
- collisions and load factor
- integer/byte/string hashing
- adversarial/seeded hashing motivation
- separate chaining
- rehashing and geometric bucket growth
- expected vs worst-case hash-table complexity
- Set vs Map ADT
- open addressing and linear probing
- tombstones and probe continuity
- clustering
- owned vs borrowed string keys
- cached string hashes

Structures Implemented:
- all previous structures
- hash_u64_mix / FNV-1a byte hashing utilities
- IntIntHashTable using separate chaining
- IntHashSet wrapper
- StringIntHashMap using open addressing + linear probing + tombstones

Tests Added:
- hash/equality/bucket tests
- distribution experiments
- 30,000-step chaining Hash Table differential test
- 20,000-step IntHashSet randomized test
- 25,000-step StringIntHashMap differential test
- tombstone/owned-key tests

Benchmarks Added:
- hash distribution
- Hash Table vs linear lookup
- Hash Set vs linear membership

Known Dependencies:
- Chapter 016 introduces tree vocabulary/invariants
- Chapter 017 implements binary-tree representation
- Chapter 018 covers recursive/iterative traversals

Open Problems:
- none if Batch 05 CI passes

Cross References:
- 013 hashing fundamentals → 014 collision resolution
- 014 Map representation → 015 Set wrapper
- 014 chaining ↔ 015 open addressing trade-offs
- 002 locality → open-addressing slot locality
- 004 expected/amortized analysis → hash operation claims
- 071–073 later revisit advanced hashing strategies

Coverage: 15 / 170 chapters
