# Pitfalls — Aho-Corasick

- rebuilding from root on every mismatch
- copying all inherited outputs unnecessarily
- forgetting suffix terminal matches
- letting failure links stay uninitialized
- treating duplicate byte patterns as one pattern ID
- accepting empty patterns without defining boundary semantics
- claiming O(T+Z) while using expensive transition lookup without qualification
