# Lab 017 — Own the Nodes

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch017_binary_tree_tests

## Part A — Construct

Build:

        10
       /  \
      20  30
     / \
    40 50

Check:
- size
- parent pointers
- height
- validator

## Part B — Foreign Node

Create second tree.
Try using second-tree root as parent in first tree.

Operation must fail without mutating either tree.

## Part C — Remove Subtree

Remove node 20.
Predict:
- new size
- height
- root children

Do not dereference old node-20 handle after removal.

## Part D — Skew

Build right-only chain and compare height with a level-filled tree of similar size.

## Part E — Sanitizers

Run full sanitizer CTest and confirm subtree removal/destruction leak no nodes.
