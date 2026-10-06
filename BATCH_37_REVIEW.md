# Batch 37 — Negative Review and Audit

## Scope

- 109 Cache-Oblivious Data Structures
- 110 External-Memory Data Structures
- 111 Disk-Based Data Structures

## Chapter 109 Review

Checked:
- representation receives no cache-line size, cache capacity or tile dimensions
- padded side is next power of two >= max(rows, cols)
- side² and byte allocation are overflow checked
- Morton index interleaves row/column bits within size_t shift width
- logical writes never touch padding
- full validator requires all padded cells to remain zero
- dense copy, row-order sum, physical Z-order sum and transpose agree

Claims kept narrow:
- Morton layout provides multiscale spatial clustering but is not claimed to outperform row-major on every workload.
- padding can be substantial for highly rectangular shapes and is reported as a space trade-off.

## Chapter 110 Review

Checked:
- build input must be globally strictly increasing
- block count is ceil(N/B)
- every block metadata range matches physical value endpoints
- directory search does not increment modeled data-block I/O
- point query examines at most one candidate data block after directory lookup
- range scan increments one logical read per overlapping block
- build writes equal produced block count
- I/O counters are instrumentation and do not affect logical set contents

Claims kept narrow:
- this is an in-RAM simulator of an external-memory cost model.
- logical block reads/writes are not asserted to equal OS/device physical I/O.

## Chapter 111 Review

Checked:
- file representation does not fwrite/fread raw C structs
- header defines magic, version, record count and record size
- integers are encoded little-endian explicitly
- signed int64 values use canonical two's-complement numeric encoding instead of copying host signed representation
- uint64 encoded record count is converted to size_t with a round-trip representability check
- record offsets are bounded before conversion to off_t
- open rejects bad magic, bad version/record size, nonzero reserved header bytes and truncated/oversized files
- validation checks strictly increasing keys
- validation does not alter public logical seek/read counters
- range scan performs binary lower_bound probes then one seek followed by sequential fread
- tests close and reopen the file before querying persistence

Audit finding fixed before commit:
- initial signed-value serialization copied the host int64 bit pattern. That was too weak for the documentation's portability claim, so it was replaced with explicit canonical two's-complement conversion.
- file creation removes a partially written target on ordinary write/flush/close failure.

Claims kept narrow:
- ordinary create/fclose is not called crash-atomic or fully durable.
- logical seeks/record reads do not equal guaranteed physical device operations because stdio, filesystem and OS caches intervene.
- this chapter is a static sorted disk index, not yet a database B+ tree or transactional storage engine.

## Verification Plan

Compiler:
```text
-std=c17 -Wall -Wextra -Wpedantic -Werror
```

Batch 37 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 111 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
