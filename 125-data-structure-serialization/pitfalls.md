# Common Mistakes

- `fwrite(&struct, sizeof struct, 1, f)` แล้วคิดว่า portable.
- ลืม padding/endianness.
- เชื่อ count จาก inputก่อนเช็ก multiplication overflow.
- ยอม trailing bytesโดยไม่ตั้งใจ.
- checksum header/payloadไม่ตรง specification.
- cast `uint64_t` ที่เกิน `INT64_MAX` เป็น `int64_t` แล้ว assume semanticsเหมือนกันทุก implementation.
- ใช้ CRC32เป็น security MAC.
