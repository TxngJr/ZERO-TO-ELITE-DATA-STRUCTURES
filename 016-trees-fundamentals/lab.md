# Lab 016 — Shape Before Code

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch016_tree_math_tests

## Part A — Classify

Classify several trees as:
- full
- perfect
- complete
- skewed
- balanced under a stated definition

## Part B — Height Bounds

Run:

    ./build/ch016_tree_math_demo

For n=1,2,3,7,8,15,16 compare minimum and maximum possible edge-height.

## Part C — Prove n-1 edges

Write a proof using parent-edge counting.

## Part D — Structural Induction

Prove recursively that:

    size(node)=1+size(left)+size(right)

counts every node exactly once.

## Part E — Shape Cost

Compare 15-node perfect-like tree vs 15-node chain:
- height
- recursive depth
- root-to-leaf operation bound
