# Common Mistakes

- ใช้ ring นี้กับหลาย producers หรือหลาย consumers.
- เรียก caller retry loop ว่า bounded wait-free operation.
- ใช้ capacity ทุก slot แต่ยังใช้ head==tail เป็น empty โดยไม่มี extra state.
- Publish tail ก่อนเขียน payload.
- Overwrite slot ก่อนเห็น consumer head advance.
- สมมติ atomics lock-free ทุก platform.
- คิดว่า wait-free = zero latency หรือ always-success.
