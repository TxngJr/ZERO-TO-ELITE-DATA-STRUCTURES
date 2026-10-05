# Theory — Order Statistics

## Rank/select duality

For sorted unique keys:

    select(rank(key)) == key

when key exists.

rank answers:
    how many keys come before this value?

select answers:
    which key occupies this sorted position?

## Correctness of select

At node, every key in left subtree is smaller than node.key.

If left subtree contains L keys:
- ranks [0,L) live left
- rank L is node
- larger ranks live right after subtracting L+1

## Correctness of rank

Whenever key > node.key, every key in left subtree and node itself is less than key.

Their count is:

    subtree_size(left)+1

which can be accumulated in O(1) per visited tree level.
