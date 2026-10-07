# Exercises

## Beginner
1. ทำไม wire record 20 bytesแม้ `sizeof(DsRecord)` อาจมากกว่า?
2. Big-endianคืออะไร?
3. Magic/versionช่วยอะไร?
4. CRC32ตรวจอะไร?
5. Trailing bytesควรรับหรือ rejectใน format นี้?

## Intermediate
6. เพิ่ม optional metadata section.
7. เพิ่ม version 2.
8. เพิ่ม varint record count.
9. เพิ่ม streaming encoder.
10. เพิ่ม file read/write wrappers.

## Advanced
11. เพิ่ม backward-compatible decoder.
12. เพิ่ม schema evolution.
13. เพิ่ม cryptographic hash/MAC.
14. เพิ่ม zero-copy read-only viewบน mmap.
15. เพิ่ม fuzz harness.

## Challenge / Research
16. เปรียบเทียบ custom binaryกับ Protobuf/FlatBuffers/Cap'n Proto.
17. วิเคราะห์ canonical serializationสำหรับ hashing/signatures.
18. เชื่อม Chapter 126 alignment: wire layoutไม่จำเป็นต้องเหมือน memory layout.
