# Theory — Aho-Corasick

Aho-Corasick is a trie plus suffix-fallback machinery.

Failure links make the automaton reuse the longest suffix that is still a known trie prefix instead of restarting matching from scratch.

Output links separate:
- structural fallback states
from
- states that actually terminate patterns

This is especially useful when many suffix states are nonterminal.
