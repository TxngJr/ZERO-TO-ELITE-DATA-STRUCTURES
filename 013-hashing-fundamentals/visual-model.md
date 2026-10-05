# Visual Model — Hashing

## Pipeline

key
 |
 v
hash(key)
 |
 v
64-bit value
 |
 | reduce modulo m
 v
bucket index

## Collision

"cat" ---> bucket 3
"tac" ---> bucket 3

keys ต่างกัน แต่ bucket เหมือนกัน.
ต้อง compare actual keys ต่อ.

## Load Factor

n=8, m=4

bucket 0: **
bucket 1: *
bucket 2: ***
bucket 3: **

alpha=2.0 ซึ่งเป็นไปได้ใน separate chaining.

## Poor direct mapping

m=8
keys 0,8,16,24

key % 8 -> bucket 0 ทั้งหมด.

mix bits ก่อน reduction ช่วย break pattern ได้.
