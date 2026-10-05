# Analysis Invariants and Assumptions

## Cost model

ใช้ definition ของ primitive cost เดียวกันตลอด derivation

## Input size

ความหมายของ n ต้องไม่เปลี่ยนกลาง proof

## Loop state

หลัง k iterations ของ linear traversal:
- ประมวลผล k elements แล้ว
- 0 <= k <= n

## Halving

หลัง k divisions ค่า current มีความสัมพันธ์กับ n/2^k ภายใต้ integer-rounding semantics ที่กำหนด

## Dynamic-array amortization

proof แบบ doubling ใช้ assumptions:
- size <= capacity
- capacity โตแบบ geometric
- resize copy live elements เดิม

ถ้า growth policy เปลี่ยน proof ต้องตรวจใหม่
