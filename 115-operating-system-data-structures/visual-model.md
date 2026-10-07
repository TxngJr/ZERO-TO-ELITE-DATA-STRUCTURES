# Visual Model

```text
Process table
slot 0: gen=4 READY  prio=1
slot 1: gen=7 BLOCKED prio=0
slot 2: gen=2 RUNNING prio=0

PID = generation:slot

Run queues
P0 -> slot 5 -> slot 9
P1 -> slot 0 -> slot 7
P2 -> ...
P3 -> ...
```

READY process ต้องอยู่ใน queue เดียว exactly once.
RUNNING มีได้ไม่เกินหนึ่ง slot ใน single-CPU model.
BLOCKED ไม่อยู่ใน ready queue.
