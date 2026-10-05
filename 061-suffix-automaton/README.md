# Chapter 061 — Suffix Automaton

## Goal

Suffix Automaton (SAM) เป็น deterministic finite automaton แบบ minimal สำหรับเซต substring ทั้งหมดของ text หนึ่งตัว.

บทนี้ implement **byte-oriented sparse-transition Suffix Automaton** รองรับ arbitrary bytes 0..255.

รองรับ:
- online text construction
- substring contains
- substring occurrence count
- distinct substring count
- longest repeated substring length
- structural validation

## State Meaning

แต่ละ state แทน equivalence class ของ substrings ที่มี **end-position set เดียวกัน**.

Metadata สำคัญ:

    max_len[state]

คือความยาวมากที่สุดของ substring ใน class นั้น.

Suffix link:

    link[state]

ชี้ไป state ของ longest proper suffix ที่มี end-position set ใหญ่กว่า.

สำหรับ non-root state v:

    min_len(v) = max_len(link(v)) + 1

ดังนั้น state v แทน substring lengths:

    min_len(v) .. max_len(v)

## Online Extension

เมื่อ append byte c:

1. create current state cur with:
       max_len[cur] = max_len[last] + 1
       terminal occurrence seed = 1

2. walk suffix links from last while transition c is absent:
       add transition -> cur

3. if root boundary reached:
       link[cur] = root

4. otherwise let q be existing transition target

   If:

       max_len[p] + 1 == max_len[q]

   then:

       link[cur] = q

5. otherwise create clone state:
       copy q transitions
       clone.max_len = max_len[p] + 1
       clone.link = q.link
       clone occurrence seed = 0

   Redirect suitable transitions from q to clone.

   Then:
       link[q] = clone
       link[cur] = clone

Clone states are the key mechanism that preserves DFA minimality while extending online.

## Sparse Transitions

A naive byte SAM could store:

    next[state][256]

That is simple but expensive.

This implementation stores each state's outgoing transitions as a dynamic array:

    (symbol, target)

Advantages:
- memory proportional to actual transitions
- easy to inspect

Trade-off:
- transition lookup is linear in out-degree

For byte alphabet:

    degree <= 256

So query complexity is:

    O(m * d)

where d <= 256.

## Occurrence Counts

During online extension:
- each newly created non-clone state gets occurrence seed 1
- clone gets 0

After build:
- order states by max_len
- propagate counts from larger max_len to suffix links

Then:

    occurrence_count[state]

equals number of end positions for any substring represented by that state.

To count a pattern:
1. traverse automaton
2. return occurrence_count of final state

## Distinct Substrings

Every non-root state v contributes:

    max_len[v] - max_len[link[v]]

new distinct substrings.

Therefore:

    distinct =
        sum over v != root
        (max_len[v] - max_len[link[v]])

This is one of the most elegant SAM formulas.

## Longest Repeated Substring

A substring is repeated if its end-position set size >= 2.

So:

    max(max_len[v])
    over states with occurrence_count[v] >= 2

gives the longest repeated substring length.

## State Bound

For text length n >= 2:

    states <= 2n - 1

including root.

Special case:

    n = 1 -> 2 states

(root + one character state).

This implementation reserves capacity dynamically and validator checks this exact linear-state bound including the singleton exception.

## Complexity

With sparse linear transition lookup:

Build:
    O(n * d) practical bound here
    standard SAM structural extension remains linear in number of redirect/link steps

For fixed alphabet and O(1) transition map:
    O(n)

Contains/count for pattern length m:
    O(m * d)

Distinct-substring count:
    O(number of states)

Longest repeated substring:
    O(number of states)

Storage:
    O(states + transitions)
    O(n) for fixed alphabet

## Suffix Automaton vs Suffix Tree

Suffix Tree:
- explicit branching over suffixes
- compressed edge labels

Suffix Automaton:
- DFA over all substrings
- states group substrings by end-position equivalence
- often very compact

## Files

- src/byte_suffix_automaton.*
- tests/test_suffix_automaton.c
- examples/suffix_automaton_demo.c
- benchmarks/suffix_automaton_benchmark.c
- standard learning artifacts
