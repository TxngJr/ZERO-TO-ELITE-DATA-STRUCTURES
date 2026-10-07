# Common Mistakes

- duplicate identifier strings everywhere instead of interning.
- use pointer identity without guaranteed interning.
- leave scope แล้วไม่ restore shadowed outer binding.
- reject shadowing across different scopes.
- allow duplicate declaration in same scope accidentally.
- invalidate current-binding table when symbol stack pops.
- def-use edge references nonexistent value.
- linked edge list forms cycle due bad next index.
