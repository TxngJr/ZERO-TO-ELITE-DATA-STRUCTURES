# Complexity — DSU

Union by size alone:
    find O(log n)

Union by size + path compression:
    amortized O(alpha(n))

alpha(n) grows so slowly that practical values are below a tiny constant, but the mathematical bound is not ordinary worst-case O(1).

Storage:
    Theta(n)

DSU is optimized for merge-only connectivity; split is not supported efficiently.
