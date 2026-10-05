# Complexity — Suffix Automaton

Classical SAM with O(1) transition access:
    build O(n)
    query O(m)

This teaching implementation uses sparse linear transition vectors:

    transition lookup O(d)
    d <= 256

So practical bounds are:

    build O(n*d)
    query O(m*d)

State/transition storage remains linear for fixed alphabet.
