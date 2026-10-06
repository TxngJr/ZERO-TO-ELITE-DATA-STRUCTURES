# Visual Model

```text
Version A                    Version B after insert(7)

    [10] A                       [10'] B
    /   \                        /    \
 [5]    [20] <-------------------------+ shared
                         [5']
                           \
                           [7]

A keeps:
[10] -> [5], [20]

B owns newly allocated path:
[10'] -> [5'] -> [7]
and shares [20]
```

Arena lifetime:

```text
arena block 0: old nodes + copied nodes
arena block 1: more copied nodes
...
all root pointers valid
until
pset_arena_free(arena)
```
