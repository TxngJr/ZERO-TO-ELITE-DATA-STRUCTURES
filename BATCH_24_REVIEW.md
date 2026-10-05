# Batch 24 — Negative Review and Audit

## Scope

- 070 Cuckoo Filter
- 071 Cuckoo Hashing
- 072 Perfect Hashing

## Chapter 070 Review

Checked:
- bucket count must be power-of-two
- fingerprint zero is reserved as empty and remapped to 1
- four slots per bucket
- two legal candidate buckets derived from key hash and fingerprint alternate relation
- direct placement is attempted before relocation
- full-table snapshot is created before kick chain
- failed relocation restores slots and RNG state
- successful insert increases occupied-slot size exactly once
- query/remove examine only the two legal buckets
- positive query is probabilistic, not exact
- deleting arbitrary false positives is documented as unsafe

Complexity:
- query/remove O(1)
- insertion bounded by MAX_KICKS and may fail
- storage O(bucket_count * bucket_size)

## Chapter 071 Review

Checked:
- full uint64_t keys are stored, including key 0
- separate occupancy arrays avoid sentinel-key restrictions
- each table1 resident validates at h1(key)
- each table2 resident validates at h2(key)
- duplicate insert has set semantics and does not grow size
- kick-chain failure restores table snapshots
- cycle/load failure triggers capacity growth and seed refresh
- rebuild collects all exact live keys before replacing old arrays
- partial allocation cleanup is transactional
- validator uses overflow-safe total-slot bound

Complexity:
- lookup/remove exact O(1)
- expected insertion O(1) amortized with occasional O(n) rebuild
- no probabilistic false positives

## Chapter 072 Review

Checked:
- input keys are sorted and deduplicated before sizing
- top bucket count equals unique-key count
- top seed retries until sum(s_i^2) <= 4n
- each nonempty bucket gets exactly s^2 secondary slots
- secondary seed retries until no stored-key collision
- exact key remains stored in slot and is compared during lookup
- empty static set is valid
- secondary total bound and validators use overflow-safe 4n calculations
- structure is explicitly static
- implementation is described as FKS-inspired and does not falsely claim the deterministic mixer supplies the formal textbook universal-hashing proof

Complexity:
- post-build lookup O(1) worst-case
- secondary storage <=4n for accepted layout
- construction uses retries and is not presented as a deterministic worst-case O(n) builder

## Testing Review

- Cuckoo Filter: 8,000 insertions, no-false-negative checks before valid deletion, remaining-key checks after deletion
- Cuckoo Hashing: 20,000 exact keys plus randomized mutations and repeated validation
- Perfect Hashing: 12,000 unique keys, duplicate inputs, exact present checks and 10,000 absent probes
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 24 is complete only when Fedora CI configures/builds Chapters 001–072 and the full CTest suite passes with sanitizers enabled.
