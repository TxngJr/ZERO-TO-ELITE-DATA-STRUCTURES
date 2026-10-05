# Visual Model — Radix Tree

Keys:
    car
    carpet
    carbon
    cat

Possible compressed shape:

             root
               |
              "ca"
               |
             node
            /    \
          "r"    "t"*
          |
        node*
        /   \
     "pet"* "bon"*

A star marks terminal boundary.
Not every byte has its own node.
