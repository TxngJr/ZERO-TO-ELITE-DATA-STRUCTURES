# Lab — Count I/Os, Not Just Comparisons

1. Build 100,000 odd keys.
2. Use B=128 keys/block.
3. Verify build writes = ceil(N/B).
4. Run 20,000 membership queries.
5. Record total logical block reads.
6. Run several range scans.
7. Compare output and expected touched-block counts.
8. Repeat for different B values.

Separate CPU timing from modeled I/O counts in your report.
