# Invariants — ByteTST

1. size equals number of stored keys including empty key when enabled
2. low subtree symbols are strictly less than node.symbol at that comparison level
3. high subtree symbols are strictly greater
4. equal subtree represents next input position
5. terminal marks a complete non-empty key
6. empty_terminal represents only zero-length key
7. no duplicate logical key
