# Implementation — Cuckoo Hashing

Two uint64_t arrays plus occupancy bytes.
Failed kick attempt restores snapshot before rebuilding.
Rebuild collects exact live keys and retries fresh seeds.
