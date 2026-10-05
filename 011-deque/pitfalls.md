# Pitfalls — Deque

- unsigned underflow from head-1
- wrong back index when size==0
- raw copy of wrapped storage during grow
- failing to reset head after grow
- failing to reset empty canonical state
- using values instead of indices in sliding-window algorithm and losing expiry information
- forgetting to expire front index
- using wrong comparison for duplicates without reasoning about expiry
- assuming nested while automatically means quadratic
- allocating n index slots and claiming auxiliary O(k) without stating allocated-memory detail
