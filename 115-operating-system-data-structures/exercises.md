# Exercises

## Beginner
1. วาด state transition READY→RUNNING→BLOCKED→READY.
2. ทำไม PID ต้องมี generation?
3. Priority 0 กับ 3 ใคร dispatch ก่อน?
4. Yield ทำอะไรกับ run queue?
5. BLOCKED process อยู่ queue ใด?

## Intermediate
6. เพิ่ม process name.
7. เพิ่ม sleep/wakeup wait queue.
8. เพิ่ม time-slice counter.
9. เพิ่ม per-priority dispatch statistics.
10. เพิ่ม aging เพื่อช่วย starvation.

## Advanced
11. เปลี่ยน run queue เป็น intrusive doubly linked list.
12. เพิ่ม multiple CPUs.
13. ออกแบบ work stealing.
14. เปรียบเทียบ strict priority กับ CFS-like ordering.
15. เพิ่ม generation overflow policy.

## Implementation
16. เพิ่ม terminate-by-state statistics.
17. เพิ่ม queue iterator.
18. เพิ่ม fault-injection tests.

## Challenge / Research
19. ศึกษา Linux task_struct/runqueue high-level design.
20. เปรียบเทียบ bitmap-assisted priority queues.
