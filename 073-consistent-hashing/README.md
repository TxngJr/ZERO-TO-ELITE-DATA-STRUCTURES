# Chapter 073 — Consistent Hashing

Consistent Hashing maps both nodes and keys onto a circular hash space. A key belongs to the first ring point clockwise from its token; lookup wraps to the first point at the end.

Virtual nodes place each physical node on the ring many times to improve load distribution.

Adding one node inserts only its virtual points. Ideally, keys move only from existing owners to the newly added node rather than being globally reshuffled. Removing that node returns affected ranges to the next clockwise owners.

Implementation stores sorted ring points and uses binary search for lookup. Node add is O(P log P) here because the simple teaching implementation appends virtual points then qsorts the full point array. Lookup is O(log P). Remove compacts O(P).

Virtual-node token collisions are tie-broken deterministically by node_id and replica. They are possible in principle because the token space is finite.

This structure is useful for sharding/cache placement, but production systems also need replication, failure domains, weights, membership protocols and migration control.
