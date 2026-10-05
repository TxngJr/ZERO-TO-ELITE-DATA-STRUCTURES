# Invariants — GraphRepr

Common:
1. endpoints valid
2. no self-loops
3. no duplicate logical edges
4. logical edge_count exact

Edge List:
5. undirected endpoints canonical

Matrix:
6. undirected cells symmetric with equal weights

Adjacency List:
7. neighbor destinations strictly increasing
8. reverse undirected arc exists with same weight
9. directed arc count equals E
10. undirected arc count equals 2E
