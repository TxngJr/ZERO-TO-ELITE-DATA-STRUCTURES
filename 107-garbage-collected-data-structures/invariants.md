# Invariants

1. generation ของทุก slot ไม่เป็น 0.
2. live_count <= capacity.
3. root_count <= live_count.
4. free_top + live_count + retired_count = capacity.
5. free-stack indices valid and unique.
6. free-stack slots ต้องไม่ alive/retired.
7. live outgoing handle ต้องเป็น null หรือ valid current handle.
8. root flag มีได้เฉพาะ alive object.
9. collection mark stack never needs more than capacity entries.
10. swept slot increments generation before reuse.
11. generation ที่ถึง UINT64_MAX ทำให้ slot retire แทนการ wrap เพื่อไม่ resurrect stale handles.
