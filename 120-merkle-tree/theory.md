# Theory — Merkle Trees

Merkle tree summarizes many leaves with one cryptographic root hash.

Security intuition:
- changing a leaf changes its leaf hash
- that changes every ancestor along its path
- root changes unless a hash collision/second-preimage event occurs

Inclusion proof needs only one sibling hash per tree level, so proof size is logarithmic rather than sending all leaves.

Merkle trees support content verification, distributed synchronization, append-only logs, blockchains and storage integrity systems. They do not by themselves provide consensus, ordering or access control.
