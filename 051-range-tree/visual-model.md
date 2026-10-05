# Visual Model — 2D Range Tree

Primary x tree:

              x=5
            /     \
          x=2     x=8
         /  \     / \
       x=1 x=4  x=7 x=9

Every node also owns:

    Y(N) = all subtree points sorted by y

Query rectangle:

    x in [3,9)
    y in [10,20)

Primary traversal finds canonical x-subtrees.

Then each Y(N) is binary-searched for [10,20).
