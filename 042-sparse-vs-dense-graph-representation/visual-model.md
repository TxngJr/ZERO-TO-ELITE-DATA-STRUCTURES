# Visual Model — Adaptive Backend

low density:

    AdaptiveGraph
         |
         v
    Adjacency List

density rises above promote threshold:

    List --convert--> Matrix

middle hysteresis band:

    keep current backend

density falls below demote threshold:

    Matrix --convert--> List

Logical graph API remains unchanged.
