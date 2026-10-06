# Batch 35 — Negative Review and Audit

## Scope

- 103 Memory Pool / Object Pool
- 104 Arena Allocator
- 105 Slab Allocator Concepts

## Chapter 103 Review

Checked:
- stride is rounded to max_align_t
- stride*capacity and metadata allocation arithmetic are overflow checked
- aligned_alloc receives a size that is a multiple of alignment
- free-list indices are stored outside user payload
- exact slot-start ownership validation uses uintptr_t range/modulo checks
- double free and foreign pointers are rejected
- live count and free count sum to capacity
- randomized 100,000-operation differential/live-count workload passes ASan/UBSan

## Chapter 104 Review

Checked:
- block raw storage includes 4095 alignment slack
- every data base is 4096-byte aligned
- requested alignment must be power-of-two <=4096
- size+alignment and block-reservation arithmetic are overflow checked
- grow path rolls back a newly linked block if placement fails unexpectedly
- mark stores block/used/aggregate/generation metadata
- reset-to-mark frees newer blocks and invalidates all prior marks by generation bump
- full reset retains one base block
- 50,000 aligned allocations and calloc zero checks pass ASan/UBSan

Audit finding fixed before commit:
- strict -Werror rejected an early compact reset implementation with -Wmisleading-indentation. Control flow was rewritten explicitly rather than disabling the warning.

## Chapter 105 Review

Checked:
- requests map to the smallest class among 16/32/64/128/256
- each slab has independent free stack, in-use bitmap and requested-size metadata
- pointer release validates owning slab and exact slot boundary
- allocator metrics check overflow before allocation publication
- requested_live_bytes and allocated_class_bytes permit direct internal-fragmentation measurement
- double-free and foreign pointers are rejected
- empty slabs can be returned by slab_trim_empty
- 80,000 randomized operations verify live/requested/class byte models

Audit findings fixed before commit:
- strict -Werror rejected compact draft control flow with -Wmisleading-indentation; source/tests were rewritten.
- complexity documentation initially said validator was O(S*K^2), but the temporary seen bitmap makes it O(S*K); documentation was corrected before commit.

## Local Verification

Compiler:
```text
-std=c17 -Wall -Wextra -Wpedantic -Werror
```

Sanitizers:
- Object Pool: ASan/UBSan + LeakSanitizer PASS
- Arena: ASan/UBSan + LeakSanitizer PASS
- Slab: ASan/UBSan + LeakSanitizer PASS

Benchmark smoke:
- Object Pool: about 1,998,848 alloc/release operations
- Arena: 1,000,000 allocations + bulk reset
- Slab: about 999,424 alloc/release operations

## Claims Kept Narrow

- Chapters 103–105 are intentionally not thread-safe.
- Chapter 105 is a teaching slab allocator; allocation/release scan slabs and do not claim production O(1) hot paths.
- Arena reset invalidates client pointers; generation validates marks, not arbitrary stale client pointers.

## Definition of Done

Batch 35 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 105 passes.
2. Existing TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
