# Batch 20 — Negative Review and Audit

## Scope

- 058 Spatial Hashing
- 059 Suffix Array
- 060 Suffix Tree

## Chapter 058 Review

Checked:
- cell_size must be positive
- negative coordinates use mathematical floor division rather than C truncation toward zero
- half-open query uses floor_div(high-1) for last touched cell
- cell arena is referenced by indices, so realloc does not invalidate chains
- table capacity remains power-of-two
- rehash rebuilds every bucket chain without moving cell point ownership
- a newly created cell is unlinked/rolled back if its first point allocation fails
- exact point predicate is applied in boundary cells
- validator checks unique chain reachability, owning-cell mapping and global point count

Complexity:
- expected insertion O(1)
- expected query O(touched cells + candidate points)
- worst case remains distribution/hash dependent

## Chapter 059 Review

Checked:
- API is byte-length based and supports embedded zero bytes
- SA is a permutation of all suffix starts
- qsort prefix-doubling complexity is documented as O(n log^2 n)
- n=1 singleton text initializes SA[0]=0 even though no doubling round runs
- doubling span avoids overflow by terminating at n
- inverse rank satisfies rank[SA[r]]=r
- Kasai LCP convention uses LCP[0]=0
- pattern comparator treats full pattern prefix as equal, enabling contiguous lower/upper search range
- empty patterns are rejected rather than silently interpreted as all positions
- validator recomputes lexicographic ordering and adjacent LCP exactly

## Chapter 060 Review

Checked:
- input bytes are widened to uint16_t and sentinel 256 cannot collide with byte data
- every suffix including sentinel-only suffix is inserted
- compressed edges store source [start,end) ranges rather than copies
- outgoing edges are selected by distinct first symbols
- node references are stable arena indices
- edge split allocates/reserves split state before publishing parent-edge mutation
- node arena capacity is linear and sized above the compressed-tree upper bound
- pattern may terminate in the middle of an edge; descendant child leaves still define occurrence set
- cached leaf_count provides count after locus search
- nonempty byte patterns cannot include sentinel-only suffix
- validator checks unique suffix leaves, distinct outgoing first symbols, exact leaf_count and exact root-to-leaf spelling of every suffix
- build is honestly documented as O(n^2) worst case; Ukkonen is introduced conceptually only

Complexity:
- structure O(n)
- naive construction O(n^2) worst case
- search O(m*d) with linear child lookup and d<=257
- report adds O(k)

## Testing Review

- Spatial Hash: 15,000 randomized rectangle queries over 6,000 points
- Suffix Array: banana, binary bytes and randomized comparisons with naive suffix sorting/search
- Suffix Tree: banana, binary bytes, repeated-character stress and randomized naive occurrence comparisons
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 20 is complete only when Fedora CI configures/builds Chapters 001–060 and the full CTest suite passes with sanitizers enabled.
