# Complexity — Bitmap

Let W=ceil(U/64).

Membership mutation:
    O(1)

Cardinality:
    O(1)

Range mutation:
    O(words touched)

Set algebra:
    O(W)

Iteration to next member:
    O(words scanned)

Storage:
    Theta(W)
