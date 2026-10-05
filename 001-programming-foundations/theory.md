# Chapter 001 Theory Notes

## Value semantics vs indirection

สมมติ function:

    void set_value(int x) { x = 7; }

ถ้า caller ส่ง int เข้าไป ค่าใน caller ไม่ได้ถูกแก้โดยตรงเพราะ parameter x เป็น object คนละตัว

แต่:

    void set_via_pointer(int *p) { *p = 7; }

caller ส่ง address มา function จึงเข้าถึง object เดิมผ่าน indirection

C++ สามารถใช้ reference:

    void set_via_reference(int& x) { x = 7; }

สิ่งที่ต้องเข้าใจไม่ใช่แค่ syntax แต่คือ identity ของ object: เรากำลังแก้ object เดิมหรือ copy?

## Scope, lifetime, storage duration

Scope ตอบว่า "ชื่อมองเห็นตรงไหน"
Lifetime ตอบว่า "object มีชีวิตอยู่ช่วงไหน"

สองอย่างนี้ไม่ใช่สิ่งเดียวกัน

ตัวอย่างอันตราย:

    int *bad(void) {
        int local = 42;
        return &local;
    }

ชื่อ local หายเมื่อออกจาก function และ lifetime ของ object ก็จบ จึงห้าม dereference pointer ที่คืนออกมา

## Struct as a representation unit

    struct Student {
        int id;
        double score;
    };

compiler อาจแทรก padding เพื่อ alignment ดังนั้น sizeof(struct Student) ไม่จำเป็นต้องเท่าผลบวก sizeof(fields) ตรง ๆ เรื่องนี้จะลงลึกใน Chapter 126

## Class invariant preview

ถ้าเรามี:

    class Percentage {
        double value;
    };

representation invariant อาจเป็น 0 <= value <= 100

constructor และ method ทุกตัวต้องรักษาเงื่อนไขนี้ มิฉะนั้น object อยู่ใน state ที่ไม่สมเหตุผล

## Generic programming

Generic code ต้องคิดเรื่อง operations ที่ type T ต้องรองรับ เช่น template max ต้องสามารถเปรียบเทียบค่าด้วย ordering ที่กำหนด

นี่คือจุดเริ่มของแนวคิด "interface/contract" ก่อนเข้าสู่ ADT ใน Chapter 003

## Cross-language snapshot

C:
- manual pointers
- struct
- ไม่มี built-in parametric generics แบบ C++ template

C++:
- pointers + references
- class/RAII
- templates/concepts

Python:
- names bind to objects
- dynamic typing
- list/dict เป็น high-level built-in structures

Rust:
- ownership/borrowing ทำให้ aliasing/lifetime ถูกตรวจจำนวนมากตอน compile

Java:
- object references + garbage collection
- generics บน type system ของ JVM language

Go:
- pointers แต่ไม่มี pointer arithmetic แบบ C
- structs/interfaces/generics

อย่ารีบตัดสินว่าภาษาใด "ดีที่สุด" โครงสร้างข้อมูลเดียวกันอาจแสดง trade-off ต่างกันเพราะ memory/ownership model ต่างกัน
