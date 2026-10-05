# Implementation — ByteBloomFilter

State:
- bit_count
- word_count
- hash_count
- insertion-call count
- packed uint64_t words

Keys are arbitrary byte sequences. Hash positions use double hashing from two mixed 64-bit hashes.
