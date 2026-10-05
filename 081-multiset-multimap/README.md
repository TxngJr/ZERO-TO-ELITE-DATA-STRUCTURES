# Chapter 081 — Multiset / Multimap

Multiset allows repeated occurrences of one key; Multimap allows multiple values associated with the same key.
This implementation compresses Multiset multiplicity as sorted `(key,count)` entries. Multimap stores sorted-by-key `(key,value)` pairs and preserves insertion order among equal keys by inserting at upper_bound.
These are different from Set/Map: duplicate semantics are part of the ADT.
