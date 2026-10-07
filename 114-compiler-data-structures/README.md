# Chapter 114 — Compiler Data Structures

Compiler ใช้ data structures หลายชุดพร้อมกัน. บทนี้ implement สองกลุ่มสำคัญ:

1. **String Interner + Lexical Symbol Table**
2. **Def-Use Graph**

## String Interner

Identifier text ถูกเก็บครั้งเดียว แล้ว code ภายในใช้ `NameId`.

```text
"count" -> NameId 17
"count" -> NameId 17
```

Interner ใช้ open-addressing hash table.

## Scoped Symbol Table

แต่ละ interned name มี pointer/index ไป binding ปัจจุบัน:

```text
current[name] -> newest symbol
                  |
                  v
             previous binding
                  |
                  v
             outer scope
```

Declare:
- intern name
- reject duplicate declaration ใน scope เดียวกัน
- push symbol
- save previous binding
- current[name] = new symbol

Leave scope:
- pop symbols ของ scope ย้อนหลัง
- restore `current[name] = previous_binding`

ดังนั้น shadowing และ unshadowing ไม่ต้อง search ทุก scope.

## Def-Use Graph

`UseDefGraph` เก็บ adjacency list จาก value definition ไป user instructions/values:

```text
v3 -> use by v8
   -> use by v11
   -> use by v20
```

ใช้ fixed edge arena เพื่อให้ insertion O(1).

## Tests

- repeated string interning returns same NameId
- root/nested scope shadowing
- duplicate declaration rejection
- leave-scope restores outer binding
- 10,000 root symbols
- 50,000 def-use edges over 10,000 values
- independent use-count reference
- full invariant validation
- ASan/UBSan
