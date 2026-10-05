# Visual Model — Trie

Keys:
    app
    apple
    apt

             root
               |
               a
               |
               p
             /   \
           p*     t*
           |
           l
           |
           e*

Prefix "ap":
matches all three.

Exact "ap":
false unless terminal marker exists there.
