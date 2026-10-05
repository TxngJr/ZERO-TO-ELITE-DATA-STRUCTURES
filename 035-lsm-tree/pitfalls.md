# Pitfalls — LSM Tree

- forgetting tombstones and resurrecting deleted values
- searching runs oldest-first
- dropping tombstone before all shadowed old versions are safe
- calling every sorted file an independent truth instead of versioned history
- ignoring read amplification
- ignoring write amplification
- assuming compaction is free
- claiming teaching in-memory run is a crash-safe SSTable
- omitting WAL when discussing durability
- implementing range scan as repeated point lookups
