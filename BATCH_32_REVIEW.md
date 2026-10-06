# Batch 32 — Negative Review and Audit

## Scope

- 094 Functional Data Structures
- 095 Copy-on-Write Structures
- 096 Concurrent Data Structures

## Chapter 094 Review

Checked:
- queue semantics are front ++ reverse(rear)
- published nodes are immutable and historical queues remain readable
- enqueue allocates one rear node without mutating the input queue
- normalization reverse-copies rear only when front is empty
- arena mark/rollback prevents publication of partial normalization on allocation failure
- full validator checks real list lengths against metadata
- operation fast path uses only shallow metadata checks
- randomized 20,000-operation queue model plus retained historical snapshots
- benchmark covers 200,000 enqueue + dequeue operations

Audit finding fixed before commit:
- initial implementation called full fq_validate inside every operation, silently turning nominal O(1) fast paths into O(n); audit separated shallow preconditions from the expensive invariant checker

Complexity:
- enqueue Theta(1)
- dequeue/peek worst Theta(r) on normalization
- amortized Theta(1) only for the documented linear-history workload
- branching old pre-normalized versions may repeat reversal work

## Chapter 095 Review

Checked:
- clone shares one backing store and increments refs
- get never detaches
- shared set/pop detach before mutation
- push detaches on sharing or capacity growth
- allocation/copy completes before new backing is published
- original clone remains unchanged after another clone mutates
- 256-clone isolation test plus 10,000-element growth path
- benchmark uses a 1,000,000-element payload, 2,000 clones and 100 first writes
- docs explicitly state that the non-atomic refcount is not thread-safe

Audit finding fixed before commit:
- AddressSanitizer/LeakSanitizer found that growth after a unique detached backing decremented the old refcount to zero without freeing it; detach now frees old storage exactly when refs reaches zero

Complexity:
- clone/get Theta(1)
- unique set/pop Theta(1)
- shared mutation Theta(n) due detach copy
- push amortized Theta(1) only on a unique backing without forced detach

## Chapter 096 Review

Checked:
- starts from thread/shared-state/data-race foundations instead of jumping to lock-free structures
- sorted-set representation invariant is protected by one pthread mutex
- readers lock as well as writers because realloc/memmove changes shared representation
- insertion allocation failure leaves old state valid
- snapshot copies one consistent locked state
- tests run 8 concurrent insert workers, 8 concurrent remove workers and duplicate-insert stress
- exact final membership and sorted snapshot are checked
- release/acquire atomic publication example demonstrates visibility/ordering separately
- docs distinguish race condition vs C data race, atomicity, visibility, ordering, compiler/CPU reordering, linearizability, CAS, ABA, lock-free and wait-free
- implementation is explicitly labeled blocking, not lock-free
- local ThreadSanitizer stress completed without race reports

Audit findings fixed before commit:
- -Werror pass caught misleading one-line control flow in early drafts and a missing <stdbool.h> dependency in the atomic example; both were corrected before publication

Complexity:
- contains O(log n) sequential work plus lock wait
- insert/remove O(n) worst due shifting plus contention
- snapshot/validate Theta(n) while holding the mutex
- asymptotic sequential cost does not include contention queueing delay

## Verification Before Commit

Local builds used:

```text
-Wall -Wextra -Wpedantic -Werror
AddressSanitizer + UndefinedBehaviorSanitizer
ThreadSanitizer for Chapter 096
```

Observed locally:
- Functional Queue tests PASS
- COW Vector tests PASS with no LeakSanitizer report
- Concurrent Set tests PASS
- release/acquire publication demo PASS
- Chapter 096 ThreadSanitizer PASS
- benchmark smoke tests PASS
- no TODO/FIXME/PLACEHOLDER in Batch 32 files
- exactly 20 exercises and 15 quiz questions per chapter

## Definition of Done

Batch 32 is complete only when:
1. Fedora ASan/UBSan workflow configures and builds Chapters 001–096.
2. Full CTest suite passes.
3. Separate Fedora ThreadSanitizer job builds and passes Chapter 096 tests.
4. COURSE_STATE, COVERAGE_MATRIX, ROADMAP, README, GLOSSARY and CHANGELOG are updated.
