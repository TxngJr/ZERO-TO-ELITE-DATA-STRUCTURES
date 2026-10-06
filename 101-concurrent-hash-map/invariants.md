# Invariants

1. bucket_count >= 4 and power of two.
2. every node resides in bucket `hash(key)&(bucket_count-1)`.
3. no duplicate key exists in a bucket chain.
4. atomic size equals total nodes in quiescent validation.
5. all bucket-array pointer/count reads occur while holding table lock.
6. chain reads/writes occur while holding that bucket mutex.
7. resize holds table write lock for entire migration/publication.
8. old bucket array is destroyed only while no table readers exist.
9. resize allocation failure leaves current table untouched.
