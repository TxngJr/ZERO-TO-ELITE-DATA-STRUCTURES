# Implementation — IntSparseTable

Fields:
- count
- levels
- logs[count+1]
- flattened table[levels*count]

API:
- create/free
- size/levels
- range_min
- validate

No mutation API exists because this chapter models immutable static RMQ.
