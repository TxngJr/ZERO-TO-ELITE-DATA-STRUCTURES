# Chapter 080 — Ordered Map / Ordered Set

Ordered containers expose sorted-key iteration and lower-bound style navigation. This teaching implementation uses sorted dynamic arrays.
Lookup/lower_bound O(log n); iteration has excellent locality; insert/remove require shifting and are O(n).
Map stores unique key/value pairs; Set stores unique keys.
