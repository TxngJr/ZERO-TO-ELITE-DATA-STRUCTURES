# Batch 13 — Negative Review and Audit

## Scope

- 037 Disjoint Set / Union-Find
- 038 Graphs Fundamentals
- 039 Graph Representations

## DSU Review

Checked:
- every element begins as its own root
- union-by-size attaches smaller component under larger root
- redundant union does not decrement component count
- path compression changes representation, not partition semantics
- component size is authoritative only at roots
- invalid indices are rejected
- validator checks parent bounds, cycle termination, root sizes, total size and root/component count
- randomized test is differential against a slow relabeling partition model
- corrected invalid randomized-test hex seed before commit

Complexity wording:
- find/union are O(alpha(n)) amortized with both heuristics
- not described as literal mathematical worst-case O(1)
- teaching validator is intentionally expensive and not part of production DSU hot path

## Graph Fundamentals Review

Checked:
- fixed vertex count preserves isolated vertices
- simple-graph contract rejects self-loops and parallel duplicates
- undirected edges are canonicalized once
- directed reverse edge is distinct
- undirected degree sum test enforces 2E
- directed in/out degree sums each enforce E
- edge weight is metadata and does not imply direction
- edge-list baseline is explicitly presented as a baseline, not optimal universal representation

## Graph Representation Review

Checked:
- all three layouts expose the same logical graph contract
- undirected Edge List stores one canonical logical record
- Matrix stores symmetric physical cells but one logical edge
- Adjacency List stores two symmetric arcs but one logical edge
- sorted adjacency vectors have unique destinations
- reverse undirected weights must match
- directed representation stores only outgoing arc/cell
- neighbor visitor does not promise universal ordering
- memory-byte estimate excludes allocator metadata
- matrix allocation guards V*V overflow
- cross-representation randomized replay verifies add/remove/lookup/neighbor equivalence

## Testing Review

- DSU randomized differential: 50,000 operations
- Graph Fundamentals deterministic directed/undirected degree-law tests
- Graph Representation randomized replay: 12,000 operations in each mode
- validators run periodically throughout mutation-heavy tests
- neighbor sets and edge counts are compared across all three representations

## Complexity Review

DSU:
- near-constant amortized connectivity operations via alpha(n)

Edge List:
- O(E) edge lookup

Adjacency Matrix:
- Theta(1) edge lookup, Theta(V^2) space

Sorted Adjacency List:
- O(log degree) lookup, Theta(V+E) sparse storage

No representation is labeled universally best.

## Definition of Done

Batch 13 is complete only when Fedora CI configures, builds Chapters 001–039 with warnings + ASan/UBSan and passes the complete CTest suite.
