# Invariants — Chapter 001

No standalone data structure is introduced, but invariants begin here.

Examples:

Loop sum invariant:
"After processing k elements, sum equals the sum of exactly those k processed elements."

Range object invariant:
"begin <= end"

Pointer-use invariant:
"When dereferenced, p must designate an object for which that access is valid and whose lifetime has not ended."

Recursion progress invariant:
"Each recursive step moves toward the base case."

These ideas become representation invariants for containers in Chapter 003 and formal proof tools in Chapters 138–140.
