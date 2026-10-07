# Theory — Compiler Data Structures

Compiler pipeline มักใช้:
- interned strings
- symbol tables
- scope stacks
- ASTs
- control-flow graphs
- SSA values
- def-use/use-def chains
- type tables
- constant pools

## Why Intern Strings?

Identifiers ถูกเปรียบเทียบบ่อย. Interning เปลี่ยน comparison จาก string content ไปเป็น compact integer ID.

## Why Binding Chains?

Lexical scope ต้องรองรับ shadowing:

```text
global x
  function x
    block x
```

current-binding + previous-binding chain ทำให้ lookup ได้ binding ใกล้สุด และ leave scope restore binding เก่าได้ทันที.

## Def-Use

Optimization เช่น dead-code elimination, constant propagation และ SSA rewrites ต้องรู้ว่า definition หนึ่งถูกใช้ที่ไหน. Def-use adjacency lists เป็น representation พื้นฐานของ relation นี้.
