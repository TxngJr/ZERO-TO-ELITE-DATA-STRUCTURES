# Common Mistakes

- fwrite C struct แล้วคิดว่า format portable.
- ไม่ version file format.
- ไม่ตรวจ short read/write.
- คำนวณ file offset overflow.
- binary search แล้วอ่านทั้งไฟล์ทุก probe.
- range scan ทำ random seek ทุก record.
- logical read counter = physical disk I/O แบบตรงตัว.
- เรียก ordinary fclose ว่า crash durability guarantee.
- เปิด corrupt/truncated file โดยไม่ validate header/size.
