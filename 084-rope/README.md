# Chapter 084 — Rope

Rope represents a sequence as a tree so inserts/deletes need not move one giant contiguous suffix.
This teaching implementation uses an implicit randomized treap with one byte per node. Subtree length acts like rope weight, allowing split by logical position and merge of adjacent sequences.
The one-byte leaf granularity is intentionally simple but memory-heavy. Production ropes usually store chunks/strings in leaves to reduce pointer/node overhead.
Expected tree height is O(log n) under good randomized priorities. Insert builds a temporary treap before mutating the existing root, so allocation failure leaves the rope unchanged.
