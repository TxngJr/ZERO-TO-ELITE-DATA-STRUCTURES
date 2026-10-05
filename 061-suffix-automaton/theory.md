# Theory — Suffix Automaton

A suffix automaton recognizes exactly the substring language of one text.

The central abstraction is not "one state per substring".
Many substrings collapse into one state when they share the same end-position set.

For state v:

    lengths represented =
    max_len(link(v))+1 ... max_len(v)

Clone states split one equivalence class when an online extension reveals that an existing transition target needs two different maximum-length contexts.

Occurrence propagation works because every end position represented by state v also belongs to its suffix-link ancestor.
