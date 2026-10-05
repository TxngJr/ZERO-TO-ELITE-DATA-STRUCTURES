# Complexity — IntDAG

has_edge:
    O(log outdegree)

add_edge:
    O(V+E) due cycle-prevention reachability
    plus O(outdegree) insertion shift

remove_edge:
    O(log outdegree + outdegree)

indegree:
    Theta(1)

outdegree:
    Theta(1)

topological sort:
    O(V+E)

storage:
    Theta(V+E)
