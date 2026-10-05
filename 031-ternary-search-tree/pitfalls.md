# Pitfalls — TST

- consuming input when moving low/high
- not consuming input when moving equal
- confusing whole-key BST ordering with per-character ordering
- pruning a node that still has low/high alternatives
- claiming guaranteed balanced lookup
- forgetting empty-key representation
- treating UTF-8 bytes as characters/code points
