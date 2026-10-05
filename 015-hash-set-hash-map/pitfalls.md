# Pitfalls — Hash Set / Hash Map

- marking deleted open-address slot EMPTY
- failing to reuse tombstones
- counting tombstone as live size
- duplicate key entries instead of update
- forgetting to copy borrowed string key
- double-freeing key during rehash
- recomputing position with different hash/equality contract
- letting load approach 1
- assuming hash order is insertion order
- claiming worst-case O(1)
- using strcmp semantics when app needs Unicode normalization/case folding
- retaining caller string pointer without ownership contract
