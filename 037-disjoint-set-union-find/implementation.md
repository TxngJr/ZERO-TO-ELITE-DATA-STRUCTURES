# Implementation — IntDSU

Fields:
- count
- components
- parent[]
- size[]

Public operations:
- create/free
- find with path compression
- connected
- union by size
- component_size
- component_count
- max_depth
- validate

Invalid indices return false rather than indexing memory.
