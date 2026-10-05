# Visual Model — DSU

Before union:

    0        3
   / \      /
  1   2    4

sizes:
    size[0]=3
    size[3]=2

union(2,4):

larger root 0 absorbs root 3:

        0
      / | \
     1  2  3
           |
           4

find(4) compresses:

        0
     / / \ \
    1 2  3  4
