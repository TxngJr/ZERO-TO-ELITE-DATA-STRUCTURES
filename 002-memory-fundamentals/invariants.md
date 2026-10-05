# Invariants — Chapter 002

Memory-oriented invariants used throughout the course:

1. Every dereference must target storage valid for that access.
2. Owned dynamic allocation is released exactly according to its ownership contract.
3. Array index must stay within valid bounds.
4. A pointer to an object is not enough; the object's lifetime must still be active.
5. After successful reallocation, only the returned/current pointer is used for the resized block.
6. Size calculations must avoid overflow before allocation.

These are engineering invariants rather than one container's representation invariant.
