# Lab 018 — Same Tree, Four Orders

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch018_tree_traversal_tests

## Part A — Predict

For:

        1
       / \
      2   3
     / \ / \
    4  5 6  7

write preorder/inorder/postorder/level-order before running demo.

## Part B — Stack Trace

Trace iterative inorder by writing:
- cursor
- stack
- output

at every step.

## Part C — Differential Tests

Compare recursive vs iterative DFS results on:
- perfect tree
- skewed tree
- irregular tree

Explain why matching outputs is a strong cross-check.

## Part D — Space

For perfect 31-node tree estimate:
- h
- maximum width
- recursive DFS call depth
- BFS maximum queue occupancy

## Part E — Benchmark

    ./build/ch018_traversal_benchmark

Compare recursive and iterative preorder on a level-filled tree.

Interpret timing as implementation/machine specific; complexity remains Theta(n).
