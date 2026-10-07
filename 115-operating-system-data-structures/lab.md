# Lab — Process Table and Run Queues

1. Create scheduler capacity 20,000.
2. Spawn 10,000 processes across 4 priorities.
3. Validate exact queue counts.
4. Dispatch priority-0 processes and alternate yield/block.
5. Wake blocked processes.
6. Terminate a process and reuse its slot.
7. Prove old PID is stale.
8. Run ASan/UBSan.

Benchmark:
- 40,000 ready processes
- 500,000 dispatch+yield cycles
- measure scheduler metadata throughput.
