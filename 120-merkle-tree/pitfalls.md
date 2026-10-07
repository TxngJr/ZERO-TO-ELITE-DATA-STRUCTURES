# Common Mistakes

- ใช้ non-cryptographic checksum แล้วเรียก Merkle tree cryptographic.
- ไม่ทำ domain separation ระหว่าง leaf/internal.
- odd leaf handling ฝั่ง builder/verifierไม่ตรงกัน.
- proof sibling order ซ้าย/ขวาสลับ.
- เปรียบเทียบ root แค่บาง bytes.
- คิดว่า Merkle root พิสูจน์ว่า data ถูกต้องโดยไม่รู้ trusted root.
- คิดว่า Merkle tree = blockchain/consensus.
- ไม่ตรวจ allocation overflow ของ hash levels.
