# Theory — Game Data Structures

Games often separate identity, component storage and spatial indexing.

Generation handles make entity references safe across slot reuse.
Sparse sets give:
- O(1) membership lookup
- dense contiguous component iteration
- O(1) swap-remove

A uniform spatial grid trades generality for predictable locality. It is effective when objects are roughly evenly distributed and queries are local.

The grid in this chapter is rebuilt from authoritative component data. Incremental maintenance is possible but introduces more synchronization/invariant complexity.
