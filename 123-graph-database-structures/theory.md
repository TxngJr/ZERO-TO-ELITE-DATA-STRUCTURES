# Theory — Graph Database Structures

Graph databases need more than an adjacency list:

- stable external node IDs
- dense internal IDs for compact storage
- outgoing adjacency for forward traversal
- incoming adjacency for reverse traversal
- label/property indexes for filtering
- versioning or transactional publication so indexes match authoritative records

CSR-style adjacency provides compact sequential edge ranges per node. It is especially effective for read-heavy snapshots.

This chapter uses rebuildable indexes to make the difference between mutable base records and query-optimized derived structures explicit.
