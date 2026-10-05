# Lab 044 — Defer the Update

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch044_lazy_segment_tree_tests

Tasks:
1. Build eight values.
2. Add +5 to whole array.
3. Inspect root lazy without pushing.
4. Query a subrange using carry logic.
5. Apply overlapping range add.
6. Verify against a naive array.
7. Explain why push preserves abstract state.
