# Invariants

1. Block height equals its array index.
2. Genesis previous hash is 32 zero bytes.
3. Every non-genesis previous hash equals prior block hash.
4. Every block has at least one transaction.
5. Stored tx_root equals Merkle root of canonical transaction records.
6. Stored block hash equals canonical header hash.
7. Transaction arrays are block-owned copies.
8. Block order is append-only.
9. Proof block/transaction indices remain explicit metadata.
10. Chain validation recomputes commitments rather than trusting cached values.
