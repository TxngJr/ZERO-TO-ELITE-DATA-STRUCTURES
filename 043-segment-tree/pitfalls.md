# Pitfalls — Segment Tree

- mixing inclusive [l,r] and half-open [l,r)
- wrong midpoint or child boundaries
- forgetting to pull aggregate after point update
- using O(n log n) repeated updates instead of linear build
- fake minimum identity causing overflow/sentinel bugs
- allocating only 2n slots with a recursive 1-based layout without proving capacity
- assuming Segment Tree solves range updates efficiently without lazy propagation
