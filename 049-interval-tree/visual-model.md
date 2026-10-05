# Visual Model — max_high Augmentation

Intervals:

            [10,20) max=40
             /            \
      [5,8) max=12       [25,30) max=40
          \                   \
         [7,12)               [35,40)

Query:

    [13,18)

Left subtree max_high=12 <= 13

Therefore the whole left subtree is pruned.

Root [10,20) overlaps immediately.
