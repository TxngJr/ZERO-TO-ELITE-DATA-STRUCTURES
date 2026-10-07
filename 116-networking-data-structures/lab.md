# Lab — Routing + Flow State

1. Insert default route and nested /8,/16,/24 prefixes.
2. Verify most-specific route wins.
3. Add 4,096 generated /24 prefixes.
4. Run exact lookup verification.
5. Insert 50,000 five-tuple flows.
6. Update every seventh flow.
7. Remove every even flow.
8. Validate tombstone accounting.
9. Run ASan/UBSan.

Benchmark combines 200,000 route lookups with 100,000 flow insert+lookup operations.
