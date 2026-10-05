# Invariants — IntStack

For every valid live IntStack:

1. size <= capacity
2. if capacity == 0 then data == NULL in this implementation's initialized state
3. if capacity > 0 then data designates storage sufficient for capacity int elements
4. logical elements occupy indices [0, size)
5. if size > 0, the abstract top maps to data[size - 1]

## Push preservation

Before:
    size <= capacity

If size == capacity, ensure_capacity establishes capacity >= size + 1 or returns failure with old state intact.

Then:
    data[size] = value
    size++

Therefore new size <= capacity.

## Pop preservation

Requires size > 0.
size is decremented by one, so new size remains <= capacity.
No allocation ownership changes.

These are informal preservation arguments; formal proof techniques return in Chapters 138–140.
