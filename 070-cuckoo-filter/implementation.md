# Implementation — Cuckoo Filter

Bucket size = 4.
Fingerprint 0 is reserved as empty, so computed zero maps to 1.
Insertion snapshots slots before kick chain so failure is transactional.
