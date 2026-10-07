# Lab — Layout and Aligned Storage

1. Compute layout for (1/1, 8/8, 4/4).
2. Verify offsets 0,8,16 and size 24.
3. Reorder to (8/8,4/4,1/1); verify size 16.
4. Compare padding bytes.
5. Reject alignment 3.
6. Exercise SIZE_MAX overflow cases.
7. Create 10,000 aligned elements with size 24/alignment 64.
8. Verify stride 64 and every pointer.
9. Test size 80/alignment 64 -> stride 128.
10. Run ASan/UBSan.

Benchmark:
- write/read two arrays with same logical payload but natural vs 64-byte stride
- report footprint difference; do not generalize performance from one machine.
