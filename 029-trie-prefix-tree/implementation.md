# Implementation — ByteTrie

Node:
- bool terminal
- TrieEdge *edges

Edge:
- uint8_t label
- TrieNode *child
- next

Edges per node are strictly sorted by label.

APIs use:
    const unsigned char *bytes
    size_t length

rather than relying on NUL termination.

Lexicographic visitor receives an ephemeral byte buffer valid only during callback invocation.
