# Complexity — Interval Tree

Balanced height:
    O(log n)

Exact insert/remove/contains:
    O(log n)

Any-overlap query:
    follows one pruned search path in the standard interval-search procedure.

All-overlap counting/reporting:
    output-sensitive pruning, commonly O(log n + k), but may visit more nodes for adversarial interval distributions.

Storage:
    Theta(n)
