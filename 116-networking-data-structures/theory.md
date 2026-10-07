# Theory — Networking Tables

Routing lookup differs from ordinary exact-key hashing: prefixes overlap, so lookup needs the **most specific matching prefix**.

A binary prefix trie represents this naturally. Default route lives at root; each traversed bit increases prefix specificity by one.

Flow/state tables are exact-key associative structures. Five-tuples are commonly used to identify transport flows, NAT state, firewall state or connection tracking entries.

Open addressing gives contiguous metadata and good locality but requires tombstone semantics so deletion does not break later probe chains.
