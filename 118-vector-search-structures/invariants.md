# Invariants

1. dims > 0.
2. size <= capacity.
3. vectors occupy exactly the first size rows.
4. every stored component is finite.
5. every stored norm² is finite and > 0.
6. vector ID equals append index and never changes.
7. heap size never exceeds k.
8. heap root is the worst retained candidate.
9. final results are ascending by distance, then ID.
