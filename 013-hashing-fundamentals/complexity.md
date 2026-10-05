# Complexity — Hash Functions

Let L = key byte length.

- integer mix: Θ(1) under fixed 64-bit machine-word model
- byte/string hash: Θ(L)
- bucket reduction: Θ(1)
- distribution measurement over n keys and m buckets: Θ(n+m)
- bucket-count storage: Θ(m)

Important:
expected Hash Table lookup is not simply "Θ(1)" for long string keys because hashing the key itself costs Θ(L) before collision-resolution work begins.
