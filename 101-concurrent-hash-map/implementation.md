# Implementation Notes

Bucket count is rounded to a power of two and indexing uses a mixed 64-bit hash masked by `bucket_count-1`.

Every normal operation acquires locks in one order:
`table read lock -> bucket mutex`.

Resize runs only after the inserting operation releases both locks, then obtains table write lock and rechecks the load condition. This avoids trying to upgrade a read lock while still holding a bucket mutex.

The new bucket array and mutexes are initialized before old state is modified. If allocation/init fails, the existing map remains valid.
