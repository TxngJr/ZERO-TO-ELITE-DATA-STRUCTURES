# Batch 29 — Negative Review and Audit

## Scope

- 085 Piece Table
- 086 Inverted Index
- 087 Posting List

## Chapter 085 Review

Checked:
- original bytes are copied once and never modified
- add buffer logical size only grows on successful insertion
- every piece references ORIGINAL or ADD with in-range start/length
- zero-length pieces are never stored
- insert splits at most one containing piece around the insertion point
- erase clips/removes every overlapping piece in the half-open erased range
- adjacent same-source contiguous pieces are coalesced
- piece-layout allocation occurs before logical content commit
- add-buffer capacity may grow before final commit, but add_size/document contents remain unchanged if the later operation fails
- sum of piece lengths equals logical size
- randomized differential edits compare full reconstructed bytes after every operation
- byte offsets are explicitly not presented as Unicode character offsets

Complexity:
- flat piece lookup/edit O(piece_count)
- copy O(text size)
- backing storage includes unreachable historical add bytes

## Chapter 086 Review

Checked:
- tokenizer accepts ASCII alphanumeric runs only
- A-Z normalization is explicit and locale-independent
- query terms use the same lowercase normalization
- each term owns a sorted unique uint64 document list
- repeated term occurrences in one document do not duplicate the document ID
- term entries remain in their hash bucket after rehash
- rehash trigger uses subtraction-based 75% threshold to avoid multiplication overflow
- Boolean AND uses two-pointer intersection
- validator checks normalized term alphabet, strict doc ordering and term_count
- add_document is not a strong transaction across all terms: allocation failure may leave earlier terms from that document indexed; this limitation is documented by the teaching implementation scope

Complexity:
- expected hash lookup O(1) plus term comparison
- sorted doc insertion O(df) shift
- AND O(df_a + df_b)

## Chapter 087 Review

Checked:
- stored uint32 doc IDs are strictly increasing and unique
- arbitrary insertion uses binary lower_bound
- intersection output remains sorted and unique
- first encoded value is the absolute first doc ID
- later values are nonnegative gaps from the previous ID
- encoder precomputes required byte count before writing
- varint decoder rejects truncated inputs
- fifth byte rejects values wider than uint32
- cumulative gap addition checks UINT32 overflow
- post-first zero gaps are rejected because decoded IDs must strictly increase
- encode/decode round-trip is tested across 5,000 docs
- malformed streams are explicitly tested

Complexity:
- contains O(log n)
- insertion O(n) worst case
- intersection O(n+m)
- encode/decode O(n)

## Testing Review

- Piece Table: 20,000 randomized byte edits with continuous full-model comparison
- Inverted Index: normalization, ordering, intersection and hash-table growth
- Posting List: exact intersection, compression round-trip and malformed varint rejection
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 29 is complete only when Fedora CI configures/builds Chapters 001–087 and the full CTest suite passes with sanitizers enabled.
