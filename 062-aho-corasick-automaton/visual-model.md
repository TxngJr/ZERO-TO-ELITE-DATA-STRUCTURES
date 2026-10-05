# Visual Model — Failure Links

Patterns:

    he
    she
    his
    hers

Trie edge path:

    root -s-> sh -e-> she

Failure of "she" state eventually reaches:

    he

So scanning "she" reports:

    she
    he

without rescanning text.
