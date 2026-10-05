# Pitfalls — Spatial Hashing

- using truncating division for negative coordinates
- forgetting exact filtering in boundary cells
- choosing cell size blindly
- using unstable pointers into a reallocating cell arena
- losing chain links during rehash
- iterating x_high/S instead of floor((x_high-1)/S) for half-open queries
- assuming expected O(1) means worst-case O(1)
