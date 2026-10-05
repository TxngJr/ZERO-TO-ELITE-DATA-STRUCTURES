# Complexity — Adaptive Graph

Adj List phase:
- has edge O(log degree)
- neighbors Theta(degree)
- space Theta(V+E)

Matrix phase:
- has edge Theta(1)
- neighbors Theta(V)
- space Theta(V^2)

Conversion:
- List -> Matrix: O(V^2 + E) conceptual allocation/replay cost
- Matrix -> List: O(V^2) scan plus list insertions

Mutation triggering conversion pays conversion cost.

Hysteresis aims to make conversions infrequent enough to amortize them over a workload phase.
