# Implementation — PersistentSegmentTree

Node pool entry:
- left child index
- right child index
- sum
- min

Tree:
- element count
- growable node pool
- growable version-root array

Node index 0 is invalid/null.

Public API:
- create/free
- size/version_count/node_count
- point_set(base_version,...)
- point_get(version,...)
- range_sum/range_min(version,...)
- validate

All published nodes are immutable.
