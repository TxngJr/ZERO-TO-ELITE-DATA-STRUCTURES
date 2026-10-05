# Chapter 082 — Circular Buffer / Ring Buffer

A ring buffer reuses a fixed contiguous array by wrapping logical positions around the physical end.
This chapter uses reject-on-full FIFO semantics: push fails when size==capacity rather than overwriting old data.
Logical element i lives at (head+i) mod capacity; implementation avoids overflow-prone raw addition by splitting at the remaining suffix.
Push/pop/front/back/get are O(1), storage is Theta(capacity).
