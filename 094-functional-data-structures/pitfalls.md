# Common Mistakes

- เรียก full validator ทุก operation → hidden Θ(n).
- reverse rear in-place → ทำลาย historical versions.
- อ้าง worst-case O(1) dequeue → strict queue มี reversal spike.
- อ้าง amortized O(1) โดยไม่ระบุ linear-history assumption.
- คิดว่า arena คือ GC → arena reclaim ได้เฉพาะ bulk lifetime.
