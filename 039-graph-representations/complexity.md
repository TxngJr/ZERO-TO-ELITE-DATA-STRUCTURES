# Complexity — Representations

Let:
- V = vertices
- E = logical edges
- d = degree/out-degree

Edge List:
- has O(E)
- neighbor scan O(E)
- space O(E)

Matrix:
- has Theta(1)
- neighbor scan Theta(V)
- space Theta(V^2)

Sorted Adj List:
- has O(log d)
- neighbor scan Theta(d)
- mutation O(d) due shifts
- space O(V+E)
