# Lab — Fixed Object Reuse

1. Create pool: object=24 bytes, capacity=1024.
2. Run 100,000 randomized allocate/free operations.
3. Maintain independent live-pointer model.
4. Reject double free and foreign pointer.
5. Verify a released slot can be reused.
6. Run ASan/UBSan + LeakSanitizer.

Benchmark:
- pool capacity 4096
- roughly 2,000,000 alloc/release operations
- compare later with malloc/free baseline.
