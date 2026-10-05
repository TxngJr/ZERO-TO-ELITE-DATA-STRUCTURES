# Complexity — Aho-Corasick

With O(1) transitions, classical AC:

Build:
    O(total pattern length + automaton setup)

Search:
    O(text length + matches)

This teaching implementation uses sparse linear edge lookup:

    edge lookup O(d), d<=256

so practical constants include sparse transition scans.

Storage:
    O(total pattern bytes + outputs)
