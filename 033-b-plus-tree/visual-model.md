# Visual Model — B+ Tree

              [20 | 40]
             /    |    \
      [5 10] -> [20 30] -> [40 50 60]

Internal separators 20 and 40 also appear in leaves.

Logical keys live in leaves:
    5,10,20,30,40,50,60

Leaf chain supports sequential range scan.
