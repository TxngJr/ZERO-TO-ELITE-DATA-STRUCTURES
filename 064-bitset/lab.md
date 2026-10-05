# Lab 064 — Pack 70 Booleans

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch064_bitset_tests

Tasks:
1. Create 70-bit set.
2. Set 0,63,64,69.
3. Inspect two uint64_t words in GDB.
4. Flip all bits and verify count.
5. AND two sets.
6. Find next set bit across word boundary.
7. Compare memory with 70-byte bool array.
