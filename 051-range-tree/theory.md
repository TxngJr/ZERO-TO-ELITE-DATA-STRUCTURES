# Theory — 2D Range Tree

## Layered searching

Primary structure solves the x constraint.

Associated y arrays solve the y constraint inside canonical x-subtrees.

This is a general pattern:

    one search structure
    augmented with another search structure

for each additional dimension.

## Build recurrence

Median split gives balanced primary tree.

At each node, associated y-order is obtained by merging children plus the node point.

Each level performs O(n) total merge work.

With O(log n) levels:

    Theta(n log n)

## Reporting proof idea

Canonical subtrees are disjoint in point membership.

Every point in the rectangle belongs to exactly one selected canonical subtree or boundary-node check.

Binary searching each canonical y array filters the second coordinate.
