# Pitfalls — DAG

- adding an edge without checking reverse reachability
- assuming duplicate edge should increase indegree
- forgetting indegree decrement on remove
- interpreting dependency edge direction inconsistently
- assuming topological order is unique
- using DFS tree parent count as DAG indegree
- confusing source/sink with first/last numeric vertex ID
