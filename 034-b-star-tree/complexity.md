# Complexity — B* Tree

For fixed order m and height h:

contains:
    O(h*m)

insert:
    O(h*m)

redistribution:
    O(m)

2-to-3 split:
    O(m)

height:
    O(log_m n)

inorder / validate:
    Theta(n)

Teaching remove:
    O(n log n)
because it rebuilds via B* insertion.

Production B* deletion can stay logarithmic in tree height but requires multi-sibling occupancy repair not implemented as the teaching mutation path.
