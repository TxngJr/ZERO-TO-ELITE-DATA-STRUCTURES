# Invariants — IntIntHashTable

1. bucket_count >= 8 for live table
2. buckets != NULL
3. size equals number of Entry nodes
4. each key occurs exactly once
5. each node is stored at bucket(hash(key),bucket_count)
6. each chain ends at NULL
7. no chain cycle
8. load policy is maintained after successful new-key insertion

## Update

Updating an existing key changes only value.
Bucket placement and size remain unchanged.

## Resize

New bucket array is allocated first.
Every old node is relinked according to new bucket count.
No node is freed during successful rehash.
