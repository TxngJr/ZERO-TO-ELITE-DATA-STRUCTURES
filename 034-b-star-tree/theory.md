# Theory — B* Tree

## Occupancy target

For order m divisible by 3:

    max children = m
    min children = 2m/3

Internal non-root minimum keys:

    2m/3 - 1

Higher occupancy can improve effective fanout and reduce page count.

## Redistribute before split

Ordinary B-Tree often splits immediately when a node is full.

B* first checks an adjacent sibling.
If sibling has capacity, combine the two node key sets plus parent separator and choose a new separator so both nodes fit.

This postpones allocation/split.

## 2-to-3 split

If both participating nodes cannot absorb overflow:
- combine left keys
- parent separator
- right keys
- choose two separators
- distribute remaining keys across three nodes

For m divisible by 3 the resulting child counts can satisfy the 2/3-style floor cleanly.

## Deletion theory

Deletion is harder because a node dropping below the high occupancy floor cannot always merge with one sibling: two near-minimum B* nodes may be too large to fit into one maximum-sized node.

Production algorithms may involve:
- borrowing from one sibling
- redistributing across two siblings
- merging 3 nodes into 2
- parent separator updates

The teaching rebuild deletion intentionally does not pretend this complexity is solved by ordinary B-Tree merge logic.
