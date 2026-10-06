# Pitfalls

- compressing unsorted IDs
- forgetting first-ID convention
- varint overflow/truncation
- assuming compression is free CPU-wise
- using uint32 IDs when corpus requires wider IDs without redesign
