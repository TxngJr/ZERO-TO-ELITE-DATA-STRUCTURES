# Batch 02 — Negative Review and Audit

## Scope

- 004 Complexity Analysis
- 005 Recursion & Iteration
- 006 Arrays

## Beginner review

Risk: ผู้เรียนจำ O(n) จาก loop shape โดยไม่กำหนด input size
Fix: Chapter 004 เริ่มจาก n/cost model ก่อน notation

Risk: recursion ถูกมองเป็น magic
Fix: Chapter 005 แยก call depth, total calls, base/progress และให้ดู GDB backtrace

Risk: "array = pointer"
Fix: Chapter 006 แยก array object, decay และ pointer object ชัดเจน

## CS-theory review

Covered:
- O, Ω, Θ
- little-o, little-ω
- best/average/worst
- recurrence intuition
- amortized aggregate/accounting/potential preview
- induction correspondence
- loop invariants
- dynamic-array amortization derivation

Deliberately deferred:
- deeper recurrence solving
- formal proof chapters 138–140
- advanced amortized structures chapter 131

## Systems review

Covered:
- call-stack resource model without claiming exact ABI layout
- contiguous locality
- realloc failure/invalidation
- size_t overflow checks
- row-major layout
- benchmark caveats

## API / memory-safety review

IntVector:
- opaque representation
- grow only after overflow validation
- temporary realloc commit
- memmove for overlapping ranges
- size increment after successful reserve
- explicit invalid index behavior
- shrink failure preserves valid old state

## Testing review

Added:
- exact count tests for complexity
- recursive vs iterative differential checks
- deterministic randomized IntVector differential test
- empty/boundary/invalid operations
- 10,000 randomized vector operations
- sanitizer-compatible CI targets

## Performance review

Added:
- empirical linear/quadratic scaling benchmark
- recursion vs iteration microbenchmark with explicit warning
- push-back vs front-insert vector benchmark

Benchmarks are teaching experiments, not universal claims.

## Definition-of-done verdict

Batch 02 is complete only if repository CI builds all targets and all CTest tests pass under ASan/UBSan. If CI fails, this document does not override the failure.
