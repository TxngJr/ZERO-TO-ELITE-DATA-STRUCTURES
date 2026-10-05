# Complexity — Bit Vector

Let W=ceil(n/64).

Build:
    O(n+W)

access:
    O(1)

rank1/rank0:
    O(1)

select1/select0:
    O(log W)
    plus <=64 local bit operations

Storage:
    O(W)
