# Invariants

1. magic/version/record-size are valid.
2. reserved header bytes are zero.
3. file size = 64 + count×16.
4. keys are strictly increasing.
5. every record is exactly 16 encoded bytes.
6. values preserve int64 bit pattern through little-endian encoding.
7. point lookup returns only exact-key matches.
8. range output preserves file/key order.
9. validation does not mutate public I/O counters.
