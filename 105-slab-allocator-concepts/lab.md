# Lab — Size Classes and Fragmentation

1. Create allocator with 64 slots/slab.
2. Generate requests 1..256.
3. Run 80,000 randomized allocate/free operations.
4. Track independent requested/class byte sums.
5. Verify allocator metrics every step.
6. Reject double frees.
7. Free all live objects.
8. Trim empty slabs and verify reserved payload becomes zero.
9. Run ASan/UBSan + LeakSanitizer.

Benchmark performs roughly 1,000,000 allocate/release operations across all five size classes.
