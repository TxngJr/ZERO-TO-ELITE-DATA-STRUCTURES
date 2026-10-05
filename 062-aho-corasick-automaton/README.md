# Chapter 062 — Aho-Corasick Automaton

## Goal

Aho-Corasick (AC) รวม pattern หลายตัวเข้า trie เดียว แล้วเพิ่ม:

- failure links
- output propagation

เพื่อค้นหลาย patterns ใน text pass เดียว.

บทนี้รองรับ arbitrary bytes 0..255 และ pattern IDs.

Pattern:

    bytes + length + id

รองรับ:
- duplicate byte patterns with different IDs
- binary byte 0
- match counting
- match reporting as:
      pattern_id
      end_position_exclusive

## Trie Layer

ก่อน build failure links:
- root = state 0
- each trie edge has one byte label
- terminal node stores output pattern records

Sparse transition vectors are used instead of dense 256-way tables.

## Failure Link

For state v and incoming byte c:

    fail[v]

points to the state representing the longest proper suffix of v's trie-string that is also a trie prefix.

Root children:

    fail = root

For deeper node:
1. start at fail[parent]
2. follow failure links until transition c exists
3. use that target, or root if none exists

## Output Propagation

Suppose pattern:

    "he"

ends at one state, and:

    "she"

ends deeper.

When scanning "she", reaching state for "she" must report both:
- she
- he

Implementation builds an **output link**:

    output_link[v]

to nearest suffix state with terminal outputs.

This avoids physically copying all inherited outputs into every state.

During report:
- emit direct outputs of current state
- follow output_link chain

## Duplicate Patterns

If two input patterns have same bytes but IDs 10 and 99:

    both records live on the same terminal state

A match emits both IDs.

Empty patterns are rejected because their matching semantics at every boundary require a separate policy.

## Matching Transition

For each text byte c:

    while state != root and no c edge:
        state = fail[state]

    if c edge exists:
        state = next(state,c)
    else:
        state = root

Then emit outputs from:
- state
- output_link chain

## Complexity

Let:
- P = total pattern bytes
- S = automaton states
- T = text length
- Z = number of reported matches
- d = sparse transition scan degree, <=256

Build with sparse linear transition lookup:
    O(P*d + failure-link work)

Classical dense/hash transition formulation:
    O(P + alphabet/state setup)

Search:
    O(T*d + failure traversals + Z)

For fixed alphabet and O(1) edge lookup, classical AC matching is:

    O(T + Z)

Storage:
    O(P + outputs)

## Aho-Corasick vs Suffix Automaton

Suffix Automaton:
- index one text
- query many substrings later

Aho-Corasick:
- index many patterns
- scan texts for those patterns

They solve opposite sides of the matching problem.

## Files

- src/byte_aho_corasick.*
- tests/test_aho_corasick.c
- examples/aho_corasick_demo.c
- benchmarks/aho_corasick_benchmark.c
- standard learning artifacts
