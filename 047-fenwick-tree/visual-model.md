# Visual Model — Fenwick Blocks

For n=8, internal 1-based indices:

tree[1] covers [0,1)
tree[2] covers [0,2)
tree[3] covers [2,3)
tree[4] covers [0,4)
tree[5] covers [4,5)
tree[6] covers [4,6)
tree[7] covers [6,7)
tree[8] covers [0,8)

Prefix(7):
    tree[7] + tree[6] + tree[4]

covers:
    [6,7) + [4,6) + [0,4)
    = [0,7)
