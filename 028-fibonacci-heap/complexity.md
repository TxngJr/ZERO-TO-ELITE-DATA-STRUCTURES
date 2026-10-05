# Complexity — Fibonacci Heap

peek-min:
    Theta(1)

insert:
    O(1) amortized

meld:
    O(1) amortized / actual pointer concatenation

decrease-key:
    O(1) amortized
    O(n) actual worst if cascading path is long

extract-min:
    O(log n) amortized
    O(n) actual worst with many roots

delete(handle):
    O(log n) amortized under valid-handle precondition

validate:
    Theta(n)

Storage:
    Theta(n)

These amortized statements depend on maintaining mark/cut invariants correctly.
