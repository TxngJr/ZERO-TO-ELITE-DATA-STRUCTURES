# Implementation — IntBinaryTree

## Public Handles

IntBinaryNode is opaque.
Callers cannot rewrite pointers directly.

Accessors:
- value
- parent
- left
- right

Mutators require IntBinaryTree so membership can be verified.

## belongs_to_tree

Start at node and repeatedly follow parent.

Success if tree root reached.
Fail if:
- NULL
- more than size parent steps
- chain ends elsewhere

## remove_subtree

Detach first, then recursively free.

Detaching before free ensures remaining tree no longer points into freed memory.

Removed node count is returned by recursive destructor and subtracted from size.

## validate

Recursive validation checks:
- expected parent pointer
- left != right when non-null
- count never exceeds stored size
- total count matches size

Empty tree requires root=NULL and size=0.
