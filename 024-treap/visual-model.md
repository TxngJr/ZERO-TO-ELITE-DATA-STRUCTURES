# Visual Model — Treap

Each node shown as key/priority.
Smaller priority pair = higher heap priority.

Insert:

      50/80
      /
   30/40

30/40 outranks 50/80, rotate right:

      30/40
         \
         50/80

Add 70/20:

      30/40
         \
         50/80
            \
            70/20

rotate left at 50 then 30:

         70/20
        /
      30/40
         \
         50/80

BST inorder remains:
    30 50 70
