# Chapter 001 — Programming Foundations

## เป้าหมาย

บทนี้สร้างภาษากลางที่เราจะใช้ตลอดหลักสูตร Data Structures ก่อนพูดถึง list, tree หรือ hash table เราต้องอ่าน code ที่เปลี่ยน state, เรียก function, เดิน loop, ส่ง address และประกอบข้อมูลหลาย field ได้

หลังจบบทนี้คุณควร:

- แยก value, variable, type และ object ได้
- เขียน condition และ loop โดยอธิบาย state ของแต่ละ iteration ได้
- แยก parameter แบบ value, pointer และ reference ได้
- อ่าน recursion ง่าย ๆ และวาด call chain ได้
- ใช้ pointer ใน C เพื่ออ่าน/แก้ object ผ่าน address ได้
- อธิบายความต่าง pointer กับ C++ reference ได้
- สร้าง struct/class และอธิบาย field layout ในเชิงแนวคิดได้
- เข้าใจเหตุผลของ templates/generics โดยยังไม่ผูกกับ container ใด container หนึ่ง

## 1. Mental model: โปรแกรมคือการเปลี่ยน state

ลองมองบรรทัดนี้:

    int x = 10;
    x = x + 5;

ก่อนบรรทัดสอง state คือ x=10 หลังบรรทัดสอง state คือ x=15

Data Structure ก็เป็น state ที่มีรูปแบบซับซ้อนขึ้น เช่น stack อาจมี buffer, size, capacity การเข้าใจ structure จึงเริ่มจากการอ่าน state transition ให้แม่น

## 2. Variable, value, type, object

- value คือค่าทางนามธรรม เช่น 42
- type กำหนดชุดค่าที่เป็นไปได้และ operations ที่อนุญาต
- variable คือชื่อใน source program ที่ผูกกับ object/value ตามกฎของภาษา
- object ใน C/C++ คือ region ของ storage ที่มี type/lifetime ตามมาตรฐานภาษา

อย่าจำว่า "variable คือกล่อง" อย่างเดียว เพราะเมื่อถึง pointer/reference เราต้องแยกชื่อ, object, address และ value ออกจากกัน

## 3. Control flow

if/else เลือกเส้นทาง
for/while ทำเส้นทางซ้ำ

ตัวอย่าง sum:

    sum = 0
    for x in data:
        sum = sum + x

Invariant แบบง่ายระหว่าง loop คือ "sum เท่ากับผลรวมของสมาชิกที่ประมวลผลไปแล้ว" แนวคิด invariant นี้จะกลับมาอย่างจริงจังใน Chapters 138–140

## 4. Function

Function ช่วยตั้งชื่อ transformation หรือ operation และจำกัด scope ของ state

ประเด็นที่ต้องถามทุก function:

1. input คืออะไร
2. output คืออะไร
3. state ภายนอกถูกแก้หรือไม่
4. ownership/lifetime ของข้อมูลเป็นของใคร
5. invalid input จัดการอย่างไร

## 5. Recursion preview

Recursion คือ function เรียกตัวเองโดยต้องมี:

- base case เพื่อหยุด
- progress ที่ทำให้เข้าใกล้ base case

ตัวอย่าง factorial:

    factorial(4)
      4 * factorial(3)
          3 * factorial(2)
              2 * factorial(1)
                  1

Chapter 005 จะอธิบาย call stack, recursion tree, tail recursion และการแปลง recursion เป็น iteration อย่างเป็นระบบ

## 6. Pointer ใน C

ถ้า:

    int x = 10;
    int *p = &x;

ให้แยก 3 สิ่ง:

- x คือ object ชนิด int
- &x คือ address ของ x
- p เป็น pointer object ที่เก็บ address ซึ่งชี้ไปยัง x
- *p คือการ dereference เพื่อเข้าถึง object ที่ p ชี้

ถ้าทำ:

    *p = 99;

ค่า x เปลี่ยนเป็น 99 เพราะเราเขียนไปยัง object เดียวกันผ่าน pointer

Pointer ไม่ได้แปลว่า heap เสมอ Pointer ชี้ไป stack object, heap object, array element หรือ object อื่นได้ ถ้า lifetime และชนิดถูกต้อง

## 7. Reference ใน C++

    int x = 10;
    int& r = x;
    r = 50;

r เป็น alias ของ x ใน semantics ของภาษา C++ reference ต้อง bind ตอนสร้างและโดยปกติไม่สามารถ rebind แบบ pointer

Pointer และ reference จึงไม่ควรถูกสอนว่า "เหมือนกัน แค่ syntax ต่างกัน"

## 8. Struct และ Class

Struct รวม fields ที่เป็น state เดียวกัน:

    struct Point {
        int x;
        int y;
    };

นี่คือพื้นฐานของ node, entry, edge และ metadata แทบทุก data structure

Class เพิ่ม encapsulation และ behavior แต่ Data Structure ที่ดีไม่จำเป็นต้องเป็น class เสมอ ขึ้นกับภาษาและ abstraction

## 9. Generics / Templates

ถ้า algorithm เหมือนกันแต่ type ต่าง เราไม่อยาก copy implementation:

    max(int, int)
    max(double, double)
    max(long, long)

C++ template ช่วยเขียนแนวคิดเดียวแล้ว instantiate ตาม type

Generics สำคัญมาก เพราะ production container มักต้องเก็บ type ใดก็ได้ภายใต้ constraints ที่ชัดเจน

## 10. Common mistakes

- ใช้ = แทน == ในภาษาที่ syntax เปิดทาง
- loop boundary ผิดหนึ่งตำแหน่ง
- recursion ไม่มี base case
- dereference NULL หรือ pointer ที่หมด lifetime
- return address ของ local object
- คิดว่า pointer ทุกตัว own memory
- คิดว่า reference ทำให้ object copy เสมอ
- สร้าง struct ที่ state ไม่รักษา invariant

## 11. เชื่อมไปบทถัดไป

Chapter 002 จะตอบคำถามที่บทนี้ตั้งไว้: address คืออะไรในมุมมอง process, stack/heap ต่างกันอย่างไร, allocation เกิดอะไรขึ้น, locality ทำไมมีผลต่อ performance

จากนั้น Chapter 003 จะยกระดับจาก syntax/representation ไปสู่ "contract" ของ Abstract Data Type

## Files

- theory.md — รายละเอียด reasoning และ cross-language model
- lab.md — Fedora hands-on lab
- exercises.md — 20 exercises
- quiz.md — self-check
- examples/foundations.c — C foundations
- examples/generics.cpp — C++ references/classes/templates
