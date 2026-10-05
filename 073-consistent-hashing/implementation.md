# Implementation — Consistent Hashing

RingPoint = token,node_id,replica.
Points are sorted by token/node/replica.
Lookup lower-bounds key token and wraps at end.
