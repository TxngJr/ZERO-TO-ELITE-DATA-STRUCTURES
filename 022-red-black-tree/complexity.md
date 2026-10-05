# Complexity — Red-Black Tree

Height:
    O(log n)

contains:
    O(log n)

insert:
    O(log n)
including search path + bounded rotations/recoloring.

delete:
    O(log n)
including search path + fix-up path.

inorder:
    Theta(n)

validate:
    Theta(n)

recursive destructor/validator stack:
    O(log n) for a valid Red-Black tree.

A corrupted tree may violate this bound, which is another reason validators matter.
