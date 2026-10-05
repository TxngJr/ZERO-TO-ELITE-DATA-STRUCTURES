# Pitfalls — Graph Traversal

- marking visited only when dequeue/pop, causing duplicate frontier entries
- treating DFS depth as shortest distance
- assuming exact traversal order is representation-independent
- forgetting disconnected vertices remain unvisited from one source
- mixing graph mutation with traversal state
- recursive DFS stack overflow on deep graphs
- claiming O(V+E) while using an edge-list neighbor scan
