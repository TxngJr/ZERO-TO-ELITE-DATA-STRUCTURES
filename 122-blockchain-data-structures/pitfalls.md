# Common Mistakes

- hash raw C struct bytesแล้วติด padding/endianness.
- เก็บ pointer ไป caller transaction buffer แทนการ copy.
- ใช้ transaction hash listแต่ไม่ commit tx count/header fields.
- ลืม verify previous-block link.
- คิดว่า hash chain = consensus.
- คิดว่า Merkle proof พิสูจน์ signature/ownership.
- rebuild proof tree แล้ว serialize transactionไม่เหมือนตอน append.
