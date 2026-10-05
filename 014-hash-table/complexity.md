# Complexity — Separate-Chaining Hash Table

Let:
- n=entries
- m=buckets
- alpha=n/m

Under uniform-ish hashing:

get:
    expected Θ(1+alpha)

put:
    expected Θ(1+alpha), amortized over resizing

remove:
    expected Θ(1+alpha)

With alpha bounded:
    expected Θ(1)

Worst collision case:
    Θ(n)

Resize:
    Θ(n+m_new)

Space:
    Θ(n+m)

Hashing int:
    Θ(1)

For string/object keys, add key-hash/equality costs.
