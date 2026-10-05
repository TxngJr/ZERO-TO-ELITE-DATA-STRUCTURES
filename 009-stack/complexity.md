# Complexity — Stack

Let n=current size.

## Array-backed

peek:
    Θ(1)

pop:
    Θ(1)

push:
    Θ(1) if capacity remains
    Θ(n) on resize
    Θ(1) amortized with geometric growth

space:
    Θ(capacity)

## Linked

peek/pop/push:
    Θ(1) at link-operation level

space:
    Θ(n)

Practical cost still includes allocator and cache behavior.

## Next Greater Element

For n inputs:
- n pushes total
- at most n pops total

Therefore:
    Θ(n) time
    Θ(n) auxiliary space worst case

Naive scan-right-from-every-position can be O(n²).
