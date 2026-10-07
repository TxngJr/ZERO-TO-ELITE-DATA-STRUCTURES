# Lab — Portable Binary Round Trip

1. Verify CRC32 of "123456789".
2. Build 100,000 deterministic records.
3. Compute exact output size.
4. Serialize.
5. Validate blob.
6. Deserialize.
7. Compare every logical field.
8. Reserialize and compare bytes.
9. Corrupt payload byte; expect CRC failure.
10. Try truncated/header-corrupt/trailing-byte blobs.
11. Run ASan/UBSan.

Benchmark:
- 1,000,000 records
- serialize
- validate + deserialize
- report MB/s conceptually from bytes/time.
