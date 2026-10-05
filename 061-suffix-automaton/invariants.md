# Invariants — ByteSuffixAutomaton

1. root index is 0
2. root.max_len == 0
3. root.link == SIZE_MAX
4. every non-root suffix link points to smaller max_len
5. outgoing transition symbols are unique per state
6. every transition target has greater max_len than source
7. state count <= 2*n-1 for nonempty text
8. propagated occurrence count is positive for every reachable non-root non-dead state
9. distinct-substring contribution max_len[v]-max_len[link[v]] is positive
