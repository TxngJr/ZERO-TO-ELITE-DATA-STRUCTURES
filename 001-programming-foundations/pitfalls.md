# Pitfalls — Chapter 001

- confusing pointer value with pointee value
- believing every pointer owns memory
- returning pointer/reference to an object whose lifetime ended
- missing recursion base case
- off-by-one loop bounds
- mutating global state unexpectedly
- assuming struct size equals simple sum of field sizes
- assuming reference and pointer have identical language semantics
- treating undefined behavior as "it will crash"
- using generic code without specifying required operations on type parameters

Debug strategy:
1. identify object identity
2. draw lifetime
3. draw aliases
4. step state transition
5. reproduce under warnings/sanitizers when memory is involved
