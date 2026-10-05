# Visual Model — Compressed Suffix Edges

For "banana" + sentinel:

root has outgoing first symbols such as:

    a...
    b...
    n...
    sentinel

Edges store source ranges, not copied strings:

    edge = [start,end)

If a new suffix matches part of an existing edge then differs:

old:
    parent --[abcdef]--> child

split after "abc":

    parent --[abc]--> split
                     /   \
                [def]   [new...]
