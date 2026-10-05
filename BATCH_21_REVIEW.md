# Batch 21 — Negative Review and Audit

## Scope

- 061 Suffix Automaton
- 062 Aho-Corasick Automaton
- 063 Rolling Hash Structures

## Chapter 061 Review

Checked:
- root is state 0 with max_len 0 and no suffix link
- every non-root suffix link decreases max_len
- sparse outgoing symbols are unique
- clone deep-copies transition vector
- clone occurrence seed is 0
- online extension redirects only transitions still targeting q
- occurrence propagation runs from larger max_len toward suffix links
- substring count reads propagated end-position count
- distinct-substring formula uses max_len[v]-max_len[link[v]]
- longest repeated substring requires occurrence count >=2
- theoretical state bound handles the n=1 two-state exception, then uses overflow-safe 1+2*(n-1) for n>=2
- sparse transition lookup is documented with degree factor d<=256 instead of being falsely called O(1)

## Chapter 062 Review

Checked:
- empty patterns are rejected explicitly
- binary byte 0 is a normal trie symbol
- duplicate byte patterns retain separate output IDs
- BFS computes root-child failures first
- deeper failure links always move to smaller trie depth
- output links point only to shallower terminal states
- inherited suffix matches are emitted through output-link chain without copying output arrays
- text scan never rewinds input position
- report pre-counts output capacity before writing
- validator checks unique transition labels, trie depth, failure depth and output-record total

## Chapter 063 Review

Checked:
- text is copied and prefix/power arrays have n+1 entries
- byte maps to byte+1
- chosen base/mod products stay below uint64_t overflow range
- raw range hash uses half-open [l,r)
- double-hash mismatch proves inequality
- double-hash equality is treated only as a candidate
- public exact equality verifies with memcmp
- Rabin-Karp reports only memcmp-verified windows
- exact LCP performs hash binary probes but authoritative linear byte verification handles theoretical collisions
- binary byte 0 is supported
- empty search patterns are rejected
- benchmark includes inttypes.h for PRIu64 formatting

## Testing Review

- SAM: randomized binary-safe pattern counts plus naive distinct/repeated checks on small texts
- Aho-Corasick: classic he/she/his/hers, duplicate binary patterns and randomized dictionary/text comparison
- Rolling Hash: banana, binary bytes and randomized equality/LCP/search comparison
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 21 is complete only when Fedora CI configures/builds Chapters 001–063 and the full CTest suite passes with sanitizers enabled.
