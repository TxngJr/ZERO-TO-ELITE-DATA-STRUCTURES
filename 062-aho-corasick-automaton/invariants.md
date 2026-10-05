# Invariants — ByteAhoCorasick

1. root index is 0
2. root failure link points to root
3. outgoing symbols are unique per state
4. trie child depth = parent depth + 1
5. non-root failure target depth is strictly smaller
6. output_link is SIZE_MAX or points to terminal state
7. output_link target depth is smaller
8. every registered pattern ends at exactly one terminal state
9. duplicate byte patterns may share terminal state but preserve separate IDs
