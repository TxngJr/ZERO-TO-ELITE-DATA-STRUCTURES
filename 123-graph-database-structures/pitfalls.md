# Common Mistakes

- ใช้ external 64-bit IDs เป็น array indexตรง ๆ.
- rebuild outgoing index แต่ลืม incoming index.
- mutate graph แล้ว query stale CSR.
- offsets prefix sumผิดทำ edge ranges overlap.
- label indexมี duplicate/missing node entries.
- BFSใช้ external IDsใน visited arrayแทน dense indices.
- เรียก adjacency list ธรรมดาว่า graph databaseโดยไม่มี ID/label/query indexes.
