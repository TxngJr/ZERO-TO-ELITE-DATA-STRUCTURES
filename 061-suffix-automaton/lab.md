# Lab 061 — Watch Clones Appear

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch061_suffix_automaton_tests

Tasks:
1. Build SAM for "ababa".
2. Record max_len and suffix links.
3. Identify a clone-producing extension.
4. Count "aba".
5. Compute distinct substring count by formula.
6. Find longest repeated substring length.
7. Compare counts with naive scanning.
