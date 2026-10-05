# Pitfalls — Trie

- confusing prefix path with exact stored key
- forgetting terminal marker
- deleting shared prefix nodes
- dense child arrays consuming huge memory
- treating UTF-8 bytes as Unicode characters
- assuming byte lexicographic order equals locale collation
- recursion depth for extremely long keys
- using NUL-terminated APIs when binary keys should permit zero bytes
