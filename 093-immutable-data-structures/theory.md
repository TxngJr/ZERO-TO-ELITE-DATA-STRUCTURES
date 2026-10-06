# Theory — Immutability

## Semantic Contract

Immutable หมายถึง state ที่ observable ผ่าน object ไม่เปลี่ยนหลัง construction. การซ่อน mutation ภายใน cache อาจยังรักษา logical immutability ได้ถ้า observable semantics ไม่เปลี่ยน แต่ implementation ต้องระวัง concurrency.

## Immutability vs Persistence

```text
Immutable object:
    no mutation of this value

Persistent collection:
    update produces new value/version
    old version remains accessible
```

FrozenIntSet ของบทนี้ immutable แต่ไม่มี `insert -> new set` API จึงไม่ใช่ตัวอย่าง versioned persistent update structure.

## Benefits

- reasoning ง่ายขึ้น
- aliasing ปลอดภัยขึ้น
- caching/hash reuse เป็นไปได้
- readers share state ได้ง่าย
- rollback/snapshot semantics ชัด

## Costs

- rebuild/copy cost เมื่อ data เปลี่ยน
- peak memory ระหว่าง publish new snapshot
- reclamation ของ old snapshots
- บาง representation ต้องเสีย flexibility เพื่อ optimize reads
