# Theory — Cuckoo Filter

Cuckoo Filter trades exact keys for compact fingerprints.
Each fingerprint has two candidate buckets. Relocation preserves the invariant that every stored fingerprint occupies one of its two legal buckets.
False positives come from fingerprint collisions; deletion is more practical than plain Bloom Filter because entries are explicit slots.
