# Pitfalls — Order Statistic Tree

- updating height but forgetting subtree_size
- repairing metadata in wrong rotation order
- defining rank ambiguously for missing keys
- mixing 0-based and 1-based select rank
- decrementing total size twice during two-child deletion
- treating duplicate keys as separate items without frequency metadata
- using inorder scan and still claiming O(log n) rank
