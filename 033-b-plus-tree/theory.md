# Theory — B+ Tree

## Separator copies

Internal keys are not additional logical set members.

They summarize lower bounds of right children.

Therefore duplicates between internal separators and leaf keys are expected representation duplicates, not logical duplicate records.

## Range advantage

Once the first matching leaf is found, subsequent range keys are reached by sequential leaf links.

No repeated root-to-leaf search is required.

## Internal borrow by child pointer

Because separators are derived from child minima, internal rebalancing can move child pointers between siblings and then recompute separator arrays.

This avoids confusing B-Tree median-key movement with B+ separator copies.
