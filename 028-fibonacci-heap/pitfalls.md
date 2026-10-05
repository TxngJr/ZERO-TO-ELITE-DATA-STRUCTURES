# Pitfalls — Fibonacci Heap

- consolidating on every insert and accidentally implementing Binomial-Heap behavior
- broken circular-list links
- child promoted to root but parent not cleared
- root left marked
- forgetting degree decrement on cut
- cascading from wrong parent
- stale min pointer after extract/delete
- using fixed too-small consolidation array
- claiming decrease-key worst-case O(1) instead of amortized O(1)
- using Fibonacci Heap in practice without measuring pointer/cache overhead
