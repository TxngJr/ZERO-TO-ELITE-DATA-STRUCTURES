# Pitfalls — Interval Tree

- using the wrong overlap predicate for half-open intervals
- treating touching endpoints as overlap
- updating AVL height but forgetting max_high
- copying successor key during deletion but leaving stale augmentation
- assuming max_high alone orders the tree
- rejecting intervals merely because they share one endpoint
- reporting O(log n+k) unconditionally without considering adversarial overlap distributions
