# Lab — Persist, Reopen, Query

1. Generate 50,000 sorted records.
2. Create index file.
3. Close everything.
4. Reopen using only pathname.
5. Validate header, file size and key order.
6. Run 20,000 random lookups.
7. Record logical seeks/record reads.
8. Run range scan and observe sequential-read pattern.
9. Test corrupt header rejection.
10. Remove temporary files.

Distinguish logical instrumentation from physical device I/O in conclusions.
