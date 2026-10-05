# Implementation — ByteSuffixAutomaton

State:
- max_len
- suffix link
- occurrence count
- sparse transition vector

Automaton:
- states arena
- root index 0
- last state
- text length

After online construction:
- counting-sort states by max_len
- propagate occurrence counts descending by max_len

Clone:
- deep-copies transition vector
- occurrence seed 0
