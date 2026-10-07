# Common Mistakes

- ใช้ byte trie แล้วเรียก nibble Patriciaโดยไม่บอก radix.
- split Leaf/Extension แล้วทำ path offset ผิดหนึ่ง nibble.
- สร้าง Extension length 0.
- ปล่อย single-child Branch ที่ควรถูก compress.
- hash pointer/address แทน canonical content.
- root เปลี่ยนตาม insertion orderเพราะ encodingไม่ canonical.
- สับสน MPT นี้กับ Ethereum MPT encoding โดยตรง; บทนี้เป็น teaching design ไม่ใช่ wire-compatible Ethereum trie.
