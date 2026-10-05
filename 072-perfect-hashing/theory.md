# Theory — Perfect Hashing

FKS perfect hashing spends quadratic space only inside small top-level buckets.
If top bucket sizes are s_i and sum s_i^2 is O(n), total secondary storage stays linear.
Each secondary table retries a seed until its stored keys are collision-free.
