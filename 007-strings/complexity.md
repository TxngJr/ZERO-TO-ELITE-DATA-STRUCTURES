# Complexity — Strings

Let n=current length, m=input byte count.

| Operation | Complexity |
|---|---:|
| length | Θ(1) |
| byte get via data[index] | Θ(1) |
| append char | Θ(1) amortized |
| append m bytes | Θ(m) amortized over geometric growth, plus copied prior content on rare growth |
| insert m at i | Θ(n-i+m) |
| erase k at i | Θ(n-i-k) movement |
| clear | Θ(1) |
| find naive | O((n-m+1)m) worst |
| strcmp-like compare | O(min(n,m)) until difference/end |
| strlen | Θ(n) |

## Builder vs repeated immutable concatenation

If each concatenation copies all previous content:

    1 + 2 + ... + n = Θ(n²)

A geometric builder can reduce total growth copying to Θ(n) for n appended bytes, aside from copying incoming bytes themselves.
