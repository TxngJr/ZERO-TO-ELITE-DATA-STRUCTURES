# Lab — Triple Store Indexes

1. Intern 10,000 deterministic terms.
2. Verify duplicate intern returns the same ID.
3. Add 100,000 deterministic unique triples.
4. Build SPO/POS/OSP.
5. Verify exact subject pattern counts.
6. Verify exact predicate pattern counts.
7. Verify exact object pattern counts.
8. Verify fully-bound membership.
9. Add another triple and prove stale query rejection.
10. Rebuild + validate all permutations.
11. Run ASan/UBSan.

Benchmark:
- 50,000 terms
- 500,000 triples
- three-index rebuild
- 50,000 pattern queries.
