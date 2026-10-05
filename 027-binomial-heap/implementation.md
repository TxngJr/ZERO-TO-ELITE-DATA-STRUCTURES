# Implementation — IntBinomialHeap

Node fields:
- key
- degree
- parent
- child
- sibling

Heap fields:
- head root
- size

Core helpers:
- link_trees
- merge_root_lists
- consolidate
- reverse child list
- remove root
- validate tree

Meld transfers nodes and clears source heap.

No allocations occur during meld itself.
