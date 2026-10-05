# Complexity — Edge-list Graph

E=edge count.

has_edge:
    Theta(E) worst

add_edge:
    Theta(E) duplicate check
    plus amortized O(1) append

remove_edge:
    Theta(E)

degree/in-degree/out-degree:
    Theta(E)

iterate all edges:
    Theta(E)

Storage:
    Theta(V+E) including explicit vertex count and edge array.
