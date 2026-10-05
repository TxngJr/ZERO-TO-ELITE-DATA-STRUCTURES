# Theory — AVL Tree

## Minimum nodes for a height

To achieve AVL height h with as few nodes as possible, child heights differ by at most one:

    N(h)=1+N(h-1)+N(h-2)

This recurrence follows Fibonacci growth.

Therefore height is logarithmic in node count.

## Local repair, global guarantee

After one BST insertion/deletion, only ancestors on the changed path can have different heights.

AVL repairs local imbalance as recursion unwinds.

Because each repair restores:
- BST order
- local height metadata
- local BF range

the global invariant is restored by induction up to root.

## Stored metadata vs recomputation

Without stored heights, computing BF could recursively scan subtrees and destroy O(log n) update complexity.

Cached height changes BF/update to O(1) per visited node.

This is a central augmented-tree pattern.
