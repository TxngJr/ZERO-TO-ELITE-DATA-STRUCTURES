# Invariants — GraphTraversal

1. visited vertices appear exactly once in order[]
2. unvisited parent/depth are SIZE_MAX
3. source parent is SIZE_MAX
4. source depth is 0
5. every other visited vertex has a visited parent
6. parent edge exists
7. child depth = parent depth + 1
8. frontier arrays never contain more than V simultaneously discovered vertices
