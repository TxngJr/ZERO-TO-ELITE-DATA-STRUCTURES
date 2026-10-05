# Pitfalls — Suffix Tree

- omitting a unique terminator and losing explicit suffix leaves
- using a normal byte terminator that might appear in binary input
- copying edge strings and wasting memory
- mutating an edge before allocation for a split is guaranteed
- holding raw pointers into an arena that may realloc
- claiming naive insertion is Ukkonen/linear time
- counting internal nodes instead of descendant leaves for occurrences
- assuming report positions are numerically sorted
