# Batch 03 — Negative Review and Audit

## Scope

- 007 Strings
- 008 Linked Lists
- 009 Stack

## Beginner review

Risk: learner concludes "array is pointer" again when studying C strings.
Fix: Chapter 007 distinguishes char array, pointer and string contract.

Risk: learner believes character always equals byte.
Fix: byte/code-point distinction and UTF-8 warning are explicit.

Risk: learner memorizes "linked-list insert is O(1)" without considering lookup.
Fix: complexity is split between known-node relinking and index-based APIs.

Risk: learner confuses Stack ADT with call stack.
Fix: Chapter 009 explicitly separates the concepts and links to Chapter 005.

## Correctness review

ByteString:
- length/capacity/terminator invariant
- overflow checks
- embedded zero supported as logical byte
- self-append survives realloc via source offset reconstruction
- self-insert copies aliased source before mutation
- expected self-insert sequence manually rechecked and corrected during review

Linked lists:
- empty endpoint invariants
- tail consistency
- doubly-linked forward/backward consistency
- circular list returns to head after exactly size steps
- randomized differential mutation tests

Stacks:
- size/capacity invariant
- linked node-count validation
- differential behavior against same reference sequence
- strict Next Greater duplicate semantics tested

## Performance-review correction

Initial linked-list benchmark used repeated get(i), which measures a Θ(n²) index-access workload rather than a fair Θ(n) traversal comparison.

It was replaced with:
- direct contiguous vector traversal Θ(n)
- direct node-by-node singly-list traversal Θ(n)

The documentation separately explains that repeated indexed get(i) on linked list is Θ(n²).

This prevents a misleading locality conclusion.

## Memory-safety review

Checked design for:
- realloc failure
- allocation-size overflow
- null terminators
- overlapping byte moves
- node unlink-before-free
- circular-list destruction
- empty/one-element endpoint transitions
- stack underflow

## Definition of Done

Batch 03 is complete only if Fedora CI:
- configures
- builds all targets with warnings and ASan/UBSan
- passes all CTest tests

A failing CI overrides any "complete" label in documentation.
