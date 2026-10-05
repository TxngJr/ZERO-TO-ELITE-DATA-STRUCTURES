# Implementation — IntCartesianTree

Tree owns:
- copy of values[]
- parent[]
- left[]
- right[]
- root index

Index itself is node identity.

Build:
- one monotonic stack
- each index pushed once
- each index popped at most once

API:
- root
- parent/left/right
- height
- range_min
- validate
