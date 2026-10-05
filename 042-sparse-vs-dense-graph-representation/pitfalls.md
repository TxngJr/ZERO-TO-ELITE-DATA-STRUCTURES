# Pitfalls — Adaptive Representation

- using one threshold and causing representation thrash
- converting in-place and corrupting graph on allocation failure
- replaying both directions of an undirected edge as two logical edges
- assuming density alone determines optimal layout
- ignoring temporary peak memory during conversion
- hiding conversion latency inside an allegedly constant-time add
- comparing byte estimates as if allocator overhead were included
