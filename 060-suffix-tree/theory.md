# Theory — Suffix Tree

A suffix tree is a Patricia/Radix-style compression of the suffix trie.

Every internal non-root node has branching caused by differing next symbols.

Edge compression is valid because nodes with only one continuation add no branching information.

The sentinel makes every suffix explicit and prevents one suffix from terminating inside another suffix's path.

Occurrence search works because all suffixes beginning with a pattern occupy exactly the descendant leaves beneath the pattern locus.
