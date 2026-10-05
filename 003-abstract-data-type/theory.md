# Chapter 003 Theory Notes

## Algebraic view แบบง่าย

เราสามารถอธิบาย Stack ADT ด้วย equations เชิงพฤติกรรม:

- is_empty(empty) = true
- top(push(S, x)) = x
- pop(push(S, x)) = S

แนวคิดนี้ไม่พูดถึง pointer หรือ array เลย นี่คือ "abstract" ใน ADT

## Representation function

ในวิชา formal methods เราอาจคิด representation function ที่ map concrete state ไป abstract state

ตัวอย่าง:

Concrete:
    data=[10,20,30,unused...], size=3, capacity=8

Abstract:
    Stack(bottom→top) = [10,20,30]

capacity ไม่ปรากฏใน abstract value

## Invariant boundary

เรามักกำหนดว่า public operation ทุกตัวรับ object ที่ valid และต้องคืน object ที่ valid

ระหว่าง operation implementation อาจชั่วคราวอยู่ใน intermediate state ได้ ตราบใดที่ error/return path ไม่ expose broken state

## Stronger error guarantee

ตอน grow:
1. คำนวณ new capacity
2. realloc ด้วย temporary pointer
3. ถ้าล้มเหลว return false โดย data/size/capacity เดิมยังใช้ได้
4. ถ้าสำเร็จจึง commit pointer/capacity ใหม่

นี่เป็น transactional thinking แบบเล็ก ๆ:
prepare → validate → commit

แนวคิดเดียวกันกลับมาใน database/storage chapters

## Interface segregation

อย่าใส่ operation เพียงเพราะ implement ง่าย
เช่น Stack ไม่จำเป็นต้อง expose random access get(i) เพราะจะทำให้ abstraction เปลี่ยนเป็น sequence-like interface

ADT ที่ดีเปิด operations ที่สะท้อน problem model
