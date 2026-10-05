# Implementation — IntXorTrie

Arena node:
- child[2] indices
- subtree_count
- terminal_count

Missing child = SIZE_MAX. Root = node 0.

Insert pre-reserves up to 64 additional nodes.

Remove first records/verifies the exact path root + 64 levels, then decrements counts only after existence is confirmed.
