# Theory — Point-Region Quadtree

## Recursive spatial decomposition

A Quadtree represents a 2D rectangle.

Splitting creates four disjoint child rectangles whose union equals the parent.

Unlike a BST, search routing is spatial rather than based on one total ordering.

## Why bucket leaves?

Splitting after every point can create too many nodes.

Buckets amortize node creation and improve locality.

They also let coincident points coexist without forcing infinite geometric refinement.

## Query correctness

If node bounds and query are disjoint, no descendant point can match.

If node bounds are fully inside query, every descendant point matches.

Otherwise descendants must be inspected.
