# Pitfalls — Splay Tree

- claiming worst-case O(log n) per operation
- implementing only repeated Zig and calling it standard splay
- wrong Zig-Zig rotation order
- forgetting search mutates representation
- joining left/right subtrees without splaying max(left)
- stale parent pointer after detach/join
- assuming tree is always visually balanced
- benchmarking one access instead of workload sequence
