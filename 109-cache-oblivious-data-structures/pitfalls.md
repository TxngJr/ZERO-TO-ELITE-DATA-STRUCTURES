# Common Mistakes

- เรียก cache-oblivious ทั้งที่ hard-code tile size.
- interleave bits ผิดลำดับ.
- next-power-of-two overflow.
- side² overflow.
- ลืม padding overhead.
- เขียน padding แล้ว physical sum ไม่เท่ากับ logical sum.
- สรุปว่า Morton เร็วกว่า row-major ทุก workload.
