# Chapter 079 — Linked Hash Map

Linked Hash Map combines hash lookup with a doubly linked iteration-order list.
This implementation preserves insertion order. Updating an existing key changes only its value; it does not move the key. Rehashing rebuilds bucket chains while preserving order links.
Expected lookup/put/remove O(1) under normal hashing; ordered iteration O(n).
