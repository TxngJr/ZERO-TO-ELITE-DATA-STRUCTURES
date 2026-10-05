# Pitfalls — Radix Tree

- assuming every prefix endpoint is a node
- zero-length edge after split
- losing old suffix
- forgetting key-ending-at-split
- duplicate sibling first byte
- deleting shared compressed path
- merging labels in wrong order
- strlen APIs for binary keys
- assuming Patricia means one universal representation
