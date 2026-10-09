# Invariants

1. line_size, set_count and associativity are nonzero.
2. each set valid_count <= associativity.
3. valid tags within one set are unique.
4. valid tags occupy a contiguous MRU→LRU prefix.
5. hits + misses == accesses.
6. evictions <= misses.
7. reset clears tags/counts/stats.
8. hit reorders but does not change valid_count.
9. non-full miss increments valid_count.
10. full miss increments evictions and preserves valid_count.
