# Complexity — Cuckoo Filter

Query/remove inspect two buckets: O(1).
Insert is expected O(1) but may perform up to MAX_KICKS and can fail.
Storage is bucket_count * 4 * 16 bits plus metadata.
