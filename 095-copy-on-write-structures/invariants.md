# Invariants

1. Live handle has non-NULL storage.
2. refs > 0.
3. size <= capacity.
4. capacity >= 4.
5. refs matches live handles sharing storage.
6. shared mutation detaches first.
7. failed detach leaves old handle unchanged.
8. refs→0 frees storage exactly once.
9. refs is non-atomic; no unsynchronized concurrent sharing.
