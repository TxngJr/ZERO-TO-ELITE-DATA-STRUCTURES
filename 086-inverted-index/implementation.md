# Implementation

Hash table buckets store TermEntry chains. Each entry owns normalized term bytes and a sorted unique dynamic uint64_t doc array. ASCII tokenization is intentionally explicit and locale-independent.
