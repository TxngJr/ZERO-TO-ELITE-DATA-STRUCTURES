# Implementation — ByteRadixTree

Node:
- terminal
- sorted RadixEdge list

Edge:
- owned label bytes
- label length
- child
- next

Core:
- longest_common_prefix
- edge lookup by first byte
- edge split
- exact lookup
- prefix lookup including mid-edge end
- subtree count
- recursive deletion
- opportunistic unary recompression
- lexicographic visitor
