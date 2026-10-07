# Implementation Notes

Transactions ถูก canonicalize เป็น 32-byte recordsก่อนสร้าง `MerkleTree`.

Block stores a private copy of every transaction plus computed `tx_root`, `prev_hash` and `hash`.

`bc_validate` rebuilds each block's transaction Merkle tree, recomputes root, checks previous-hash linkage and recomputes block hash.

Proof wrapper owns a copied Merkle proof; verification serializes caller transaction canonically and delegates to Chapter 120.
