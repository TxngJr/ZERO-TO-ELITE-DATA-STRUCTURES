# Exercises — Chapter 013

## Beginner
1. นิยาม hash function
2. hash value กับ bucket index ต่างกันอย่างไร
3. collision คืออะไร
4. load factor คืออะไร
5. equal keys ต้อง hash เท่ากันหรือไม่

## Intermediate
6. อธิบาย why collisions unavoidable
7. วิเคราะห์ keys multiples of 16 with m=16
8. อธิบาย string hashing cost
9. แยก table hash กับ cryptographic hash
10. อธิบาย seeded hashing motivation

## Advanced
11. อธิบาย birthday-paradox intuition
12. วิเคราะห์ power-of-two masking
13. ออกแบบ hash contract สำหรับ case-insensitive strings
14. อธิบาย adversarial collision attack concept
15. อธิบาย universal hashing preview

## Implementation
16. เพิ่ม hash function สำหรับ pair of uint64
17. เขียน histogram bucket occupancy
18. วัด low-bit distribution ของ hash output

## Challenge / Research
19. ศึกษา SipHash motivation และอธิบายว่าทำไม runtime บางตัวสนใจ keyed hashing
20. เปรียบเทียบ FNV-1a, fast non-crypto hashes และ cryptographic hashes ในมิติ goal/cost/threat model
