# Lab — Phase-Oriented Allocation

1. Create arena default block 1024 bytes.
2. Allocate 50,000 objects sizes 1..97.
3. Cycle alignment 1..4096.
4. Verify pointer alignment.
5. Create mark, allocate extra data, reset.
6. Confirm marker cannot be reused.
7. Full reset and verify one block remains.
8. Run ASan/UBSan + LeakSanitizer.

Benchmark ใช้ 1,000,000 allocations ของ 24 bytes และวัด allocation/reset แยก.
