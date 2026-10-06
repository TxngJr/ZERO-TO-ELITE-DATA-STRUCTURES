# Theory — Disk-Based Structures

Disk/storage structures must define more than an in-memory ADT:
- serialized representation
- offsets/pages/records
- persistence across process lifetime
- corruption/version checks
- random vs sequential access patterns
- crash consistency and durability policy

This chapter intentionally implements a simple sorted disk index before Chapter 112 database indexes.

## Random vs Sequential

Random seeks are typically more expensive or less prefetch-friendly than sequential scans, although SSDs and OS caches change constants dramatically.

Binary search on a flat sorted file has O(log N) record probes. A B/B+ tree reduces external-memory height by using high fanout pages; that connection is revisited in Chapter 112.
