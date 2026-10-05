# Implementation — IntSplayTree

Node:
- key
- parent
- left
- right

Core helpers:
- rotate_left/right
- splay
- find_or_last
- maximum_node
- free_subtree
- validate_node

Public contains mutates representation:
- found node is splayed
- on miss last visited node is splayed

Insert duplicates also splay the existing key before returning false.

Delete splays target to root then joins detached subtrees.
