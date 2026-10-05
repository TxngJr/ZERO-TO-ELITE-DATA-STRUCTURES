# Implementation — IntSkipList

Node:
- int key
- size_t level
- flexible array next[]

List:
- head node with max_level forward pointers
- current_level
- max_level
- size
- deterministic PRNG state

Core:
- random_level
- predecessor search
- contains
- insert
- remove
- range
- validate

The public constructor chooses max_level.
Probability is fixed at 1/2 in this chapter.
