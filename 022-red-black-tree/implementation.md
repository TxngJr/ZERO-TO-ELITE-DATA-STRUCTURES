# Implementation — IntRedBlackTree

Node:
- key
- color
- parent
- left
- right

NULL is logically BLACK.

Core helpers:
- color_of
- rotate_left/right
- transplant
- insert_fixup
- delete_fixup
- minimum_node
- validate_node

## Delete with NULL x

If replacement child x is NULL, it has no parent pointer.

Therefore remove keeps:

    x_parent

and passes both x and x_parent into delete_fixup.

Whenever x becomes a real node, its own parent pointer is used naturally.

## Validator

Checks:
- global BST bounds
- root black
- parent links
- no red node with red child
- equal left/right black-height
- reachable count == size

This catches trees that look sorted but violate balancing invariants.
