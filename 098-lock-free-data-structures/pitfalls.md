# Common Mistakes

- Free/reuse popped node immediately → use-after-free/ABA.
- เรียก CAS loop ว่า wait-free.
- สมมติว่า C atomic ทุก platform lock-free.
- ใช้ relaxed ordering โดยไม่พิสูจน์ publication.
- Maintain exact size naively หลัง publish; pop อาจ race กับ bookkeeping.
- Retry forever on exhausted fixed pool แทน returning capacity failure.
- TSan pass แล้วคิดว่าพิสูจน์ progress/linearizability.
