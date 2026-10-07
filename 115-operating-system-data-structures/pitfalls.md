# Common Mistakes

- ใช้ raw slot index เป็น PID แล้ว stale handle ชี้ process ใหม่.
- process READY แต่ไม่ได้อยู่ run queue.
- wake BLOCKED process แล้วลืม enqueue.
- yield แล้วลืม clear running slot.
- terminate READY process โดยไม่ถอดจาก queue.
- เปลี่ยน priority แต่ปล่อย node ไว้ queue เดิม.
- สมมติ strict priority มี fairness.
- เรียก model นี้ว่า thread-safe ทั้งที่ไม่มี synchronization.
