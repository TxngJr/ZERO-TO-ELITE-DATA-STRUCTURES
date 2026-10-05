# Invariants — ByteRollingHash

1. prefix arrays have length n+1
2. power[0] == 1 under both moduli
3. prefix[0] == 0
4. recurrence matches each owned text byte
5. range hash uses half-open [l,r)
6. equal byte ranges always produce equal hash pairs
7. public exact equality never trusts hash equality alone
8. Rabin-Karp reports only memcmp-verified matches
