# Complexity — Recursion & Iteration

| Operation | Time | Auxiliary space | Notes |
|---|---:|---:|---|
| factorial recursive | Θ(n) | Θ(n) call depth | constant work per level |
| factorial iterative | Θ(n) | Θ(1) | scalar state |
| array sum recursive | Θ(n) | Θ(n) | one call per element |
| array sum iterative | Θ(n) | Θ(1) | loop |
| Euclid GCD | O(log min(a,b)) | recursive version proportional to call depth | tighter analysis depends on Euclid/Lamé result |
| binary search recursive | Θ(log n) worst | Θ(log n) | one recursive half |
| binary search iterative | Θ(log n) worst | Θ(1) | scalar bounds |

## Recurrence examples

factorial:

    T(n)=T(n-1)+Θ(1)=Θ(n)

binary search:

    T(n)=T(n/2)+Θ(1)=Θ(log n)

naive Fibonacci:

    T(n)=T(n-1)+T(n-2)+Θ(1)

grows exponentially in n; overlapping work is the issue.

## Space caveat

Compiler optimization can change physical frames, but complexity analysis uses source-level abstract model unless a stronger runtime guarantee is specified.
