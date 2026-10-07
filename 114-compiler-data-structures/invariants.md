# Invariants

## CompilerTables
1. each interned string has one NameId.
2. hash slots reference valid NameIds.
3. current binding for a name is SIZE_MAX or a valid active symbol.
4. symbol previous_binding equals binding active before that declaration.
5. current binding equals newest active symbol for each name.
6. scope starts are nondecreasing and inside symbol stack.
7. duplicate declaration in same lexical scope is rejected.

## UseDefGraph
8. each head is END or a valid edge index.
9. each edge user is a valid value.
10. each next is END or a prior valid edge.
11. traversal cannot exceed edge_count.
