# Invariants — IntAVL

For every node:

1. all left keys < node.key
2. all right keys > node.key
3. keys unique
4. stored height = 1 + max(left height,right height)
5. balance factor in {-1,0,+1}

Tree-level:
6. reachable count=size
7. inorder strictly increasing
8. public edge-height = stored root height - 1

Rotations must preserve 1–3 while restoring 4–5.
