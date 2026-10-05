# Theory — Dense Bitmap

A bitmap is a characteristic function of a finite integer set:

    bitmap[x] = 1 iff x is in S

Set algebra maps directly to bitwise algebra:
- union -> OR
- intersection -> AND
- symmetric difference -> XOR
- difference -> A AND NOT B

Cached cardinality adds a derived invariant:

    cardinality = sum popcount(words)

Maintaining derived metadata is useful only if every mutation preserves it.
