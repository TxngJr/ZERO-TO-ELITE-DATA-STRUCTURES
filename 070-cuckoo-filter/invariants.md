# Invariants — Cuckoo Filter

1. bucket_count is power of two
2. fingerprint 0 means empty
3. size equals occupied slot count
4. every successful insertion contributes exactly one occupied slot
5. failed relocation restores pre-insert table
