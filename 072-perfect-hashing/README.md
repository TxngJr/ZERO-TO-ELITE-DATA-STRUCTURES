# Chapter 072 — Perfect Hashing

Perfect hashing targets a static key set and chooses hash functions so stored keys have no collisions.

บทนี้ใช้ two-level FKS-inspired layout:
1. deduplicate input keys
2. choose top seed distributing n unique keys into n buckets with sum(s_i^2) <= 4n
3. bucket containing s keys receives s^2 secondary slots
4. retry a bucket seed until its s keys occupy distinct slots

Lookup computes one top bucket and one secondary slot, then compares exact uint64_t key: O(1) worst-case after successful construction.

Storage secondary slots <= 4n for the accepted top distribution, plus top bucket metadata.

Important honesty: implementation uses a deterministic mixed 64-bit seeded family for engineering clarity. The chapter follows FKS structure and retry logic, but does not claim this exact mixer constitutes the textbook formal universal-hashing proof.

Structure is static: updates require rebuilding.
