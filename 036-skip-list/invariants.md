# Invariants — IntSkipList

1. max_level >= 1
2. 1 <= current_level <= max_level
3. level 0 contains all nodes
4. every level is strictly increasing by key
5. node appears only at levels below node.level
6. every node on upper level also exists in level 0
7. head pointers above current_level are NULL
8. top active level is non-empty unless list is empty
9. level-0 node count == size
10. keys are unique

Random-looking heights are not a correctness invariant.
