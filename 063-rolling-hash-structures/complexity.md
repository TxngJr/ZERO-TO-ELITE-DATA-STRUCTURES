# Complexity — Rolling Hash

Build:
    O(n)

Raw range hash:
    O(1)

Exact equality:
    O(1) on hash mismatch
    O(length) on hash match due to memcmp

Verified Rabin-Karp:
    expected near O(n+m)
    worst O(nm)

Exact LCP implementation:
    O(log L) hash probes + O(L) verification

Storage:
    Theta(n)
