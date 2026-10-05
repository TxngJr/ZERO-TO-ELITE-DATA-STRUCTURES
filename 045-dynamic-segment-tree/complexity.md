# Complexity — Dynamic Segment Tree

Let U be coordinate-span size.

Point add/get:
    O(log U)

Range sum:
    O(log U) canonical decomposition, often less work when NULL branches terminate

Storage:
    O(K log U) worst for K touched coordinates

Unlike static Segment Tree, memory is not Theta(U).
