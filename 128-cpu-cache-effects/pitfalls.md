# Common Mistakes

- สับสน cache lineกับ cache set.
- tag/set mathใช้ byte addressผิด.
- hitแล้วไม่ update replacement order.
- missใน setที่ยังไม่เต็มแต่ดันนับ eviction.
- ใช้ timestamp LRUแล้วไม่คิด overflow.
- สรุป simulatorนี้แทน CPUจริงทั้งหมด.
- คิดว่า row-majorจะชนะทุก geometry/workloadโดยไม่วัด.
- เรียก every missว่า compulsory miss.
