# Pitfalls — Suffix Automaton

- thinking one state equals one substring
- forgetting clone transition deep-copy
- giving clone occurrence seed 1
- failing to redirect all required transitions
- propagating occurrences in ascending max_len order
- forgetting suffix-link max_len must strictly decrease
- claiming O(1) transition lookup while using linear vectors
- confusing suffix automaton with suffix trie
