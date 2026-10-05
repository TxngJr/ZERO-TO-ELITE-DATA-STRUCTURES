# Theory — Octree

## 3D recursive partition

A node box is divided at three midplanes.

The eight child boxes are disjoint under half-open bounds and their union equals the parent.

## Branching-factor growth

Fixed binary partition in d dimensions creates:

    2^d

children.

This is already 8 in 3D, illustrating why spatial trees must control depth and allocation carefully.

## Query correctness

Subtree box is a conservative exact spatial domain:
- disjoint => no point can match
- fully contained => every point matches
- partial => recurse
