# Common Mistakes

- คิดว่า O(n) เท่ากันจึง performance เท่ากัน.
- ใช้ tile ใหญ่เกิน working set.
- ใช้ tile เล็กมากจน overhead ครอบงำ.
- ลืม edge tiles/padding.
- คูณ dimensions โดยไม่ตรวจ overflow.
- เปรียบ benchmark ที่ทำงานไม่เท่ากัน.
- สรุปจาก benchmark เดียวว่า cache-aware layout เร็วกว่าเสมอ.
- confuse cache-aware กับ cache-oblivious.
