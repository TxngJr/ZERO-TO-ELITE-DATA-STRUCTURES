# Visual Model

```text
Block 0
 prev = 00..00
 tx_root = Merkle(T0...)
 hash = H(header0)
      |
      v
Block 1
 prev = hash0
 tx_root = Merkle(T1...)
 hash = H(header1)
      |
      v
Block 2
 prev = hash1
 ...
```

Transaction proof ตรวจ membership เทียบกับ `tx_root` ของ block เป้าหมาย.
