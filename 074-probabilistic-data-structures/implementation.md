# Implementation — Probabilistic Data Structures

U64KmvSketch stores up to k unique 64-bit hashes.
Insert rejects duplicate hashes, fills until k, then replaces current maximum only when a smaller hash arrives.
Merge inserts retained hashes directly without rehashing.
