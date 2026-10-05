# Invariants — Reservoir Sampling

1. capacity > 0
2. sample_count <= capacity
3. sample_count = min(seen,capacity)
4. after n>=k each stream position has target inclusion probability k/n under ideal RNG
5. failed seen-overflow update does not mutate
