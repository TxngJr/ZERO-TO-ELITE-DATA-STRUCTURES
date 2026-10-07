# Implementation Notes

Term dictionary uses open addressing and owns copied UTF-8 byte strings. Term IDs are dense and stable.

Triple duplicates are rejected by a second open-addressed set keyed by all three term IDs.

Index arrays store triple-table indices rather than copying triples. Rebuild allocates three fresh permutations and sorts them with deterministic comparators before publication.

Pattern fields use ID 0 as wildcard because valid term IDs start at 1.

Query selects:
- SPO if subject bound
- POS if predicate bound
- OSP if object bound
- SPO full scan if all wildcard

Binary search narrows the first bound component, then residual bound fields are filtered.
