# Lab 035 — Watch Versions Accumulate

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch035_lsm_tests

Tasks:
1. Create MemTable capacity 4.
2. Put 1..4 then insert 5 to force pre-insert flush.
3. Update key 2 with newer value.
4. Delete key 3 and verify older run value stays hidden.
5. Flush several runs.
6. Measure run count before/after compaction.
7. Verify logical scan identical before/after compaction.
8. Explain where WAL would sit in a durable implementation.
