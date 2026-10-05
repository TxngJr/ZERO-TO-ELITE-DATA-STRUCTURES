# Visual Model — subtree_size

            40 (size=7)
           /           \
      20 (3)          60 (3)
      /   \           /   \
    10(1) 30(1)     50(1) 70(1)

select(4):

root left size=3

4 > 3
new rank = 4-3-1 = 0
go right to 60

60 left size=1
0 < 1
go left -> 50

answer = 50
