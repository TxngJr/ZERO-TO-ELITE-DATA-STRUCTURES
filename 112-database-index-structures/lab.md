# Lab — Primary + Secondary Index

1. Generate 50,000 rows.
2. Shuffle input order.
3. Build `DbIndex`.
4. Verify primary lookups against a dense reference.
5. Query one duplicated secondary value.
6. Query a secondary range.
7. Confirm results are ordered by (secondary, primary).
8. Run ASan/UBSan.
9. Benchmark 100,000 primary lookups and repeated secondary ranges.
