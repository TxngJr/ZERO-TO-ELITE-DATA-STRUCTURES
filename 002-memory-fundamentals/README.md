# Chapter 002 — Memory Fundamentals

## เป้าหมาย

Data Structure คือการจัดข้อมูลใน memory ภายใต้กฎบางอย่าง บทนี้จึงสร้าง model ของ memory ก่อนสร้าง container จริง

หลังจบบทนี้คุณควร:

- อธิบาย virtual address ในระดับ process ได้
- แยก stack storage กับ dynamic allocation/heap ได้โดยไม่ใช้คำอธิบายเกินจริง
- อธิบาย pointer value, pointee และ lifetime ได้
- ใช้ malloc/free อย่างสมดุล
- เข้าใจ dangling pointer, leak, double free, out-of-bounds
- วาด memory layout ของ array/struct แบบแนวคิดได้
- เข้าใจ alignment/padding preview
- อธิบาย temporal/spatial locality
- อธิบายว่าทำไม contiguous data มักเป็นมิตรกับ cache กว่า pointer chasing

## 1. Process memory model แบบย่อ

Linux process เห็น virtual address space ของตัวเอง แผนภาพเชิงแนวคิด:

    high address
    +------------------+
    | stack            |
    | ...              |
    | mapped regions   |
    | shared libraries |
    | heap/dynamic     |
    | data / bss       |
    | code / text      |
    +------------------+
    low address

นี่เป็น simplified model ไม่ควรสรุปว่าตำแหน่ง/ทิศทางเหมือนกันทุกระบบ

## 2. Address

Address ที่โปรแกรมพิมพ์ผ่าน %p คือ virtual address ภายใต้ environment ปัจจุบัน เราไม่ควรตีความว่าเป็น physical RAM address ตรง ๆ

Pointer เก็บค่า address ตาม representation ของ platform/language ABI และต้องใช้เฉพาะเมื่อ pointer นั้น valid สำหรับ operation ที่ทำ

## 3. Stack

คำว่า stack มีสองบริบท:

- call stack: logical nesting ของ active calls
- stack memory: storage ที่ implementation มักใช้เก็บ activation records/local data

อย่าเหมารวมว่า local variable ทุกตัว "ต้องอยู่ RAM stack เสมอ" optimizer สามารถเก็บค่าใน register หรือตัดออกได้ สิ่งที่ source-level reasoning สนใจคือ lifetime/storage semantics ก่อน physical placement

## 4. Heap / dynamic allocation

C:

    int *p = malloc(sizeof *p);

ถ้าสำเร็จ p ชี้ storage ที่มีขนาดพอสำหรับ int object ที่เราจะสร้าง/ใช้งานตามกฎ C

เมื่อไม่ใช้:

    free(p);
    p = NULL;

การตั้ง NULL ไม่ใช่สิ่งที่ free ทำให้เอง แต่ช่วยลด accidental reuse ของ pointer variable ตัวนั้น

## 5. Ownership

C ไม่มี ownership checker จึงต้องสร้าง convention:

- ใคร allocate?
- ใครต้อง free?
- pointer ไหน owning?
- pointer ไหน borrowing?
- lifetime ของ pointee ยาวถึงเมื่อไร?

Data Structure ที่ manual-memory หนักจะพังง่ายถ้า contract ownership ไม่ชัด

## 6. Memory safety failures

Leak:
allocate แล้วไม่มี path คืน memory

Dangling pointer:
pointer ยังเก็บ address แต่ object ที่เคยอยู่ตรงนั้นหมด lifetime

Double free:
คืน allocation เดิมมากกว่าหนึ่งครั้ง

Out-of-bounds:
เข้าถึงนอก object/array ที่อนุญาต

Use-after-free:
access allocation หลัง free

ทั้งหมดนี้อาจเป็น undefined behavior ไม่จำเป็นต้อง crash ทันที

## 7. Contiguous memory

Array ของ int:

    +----+----+----+----+
    | a0 | a1 | a2 | a3 |
    +----+----+----+----+

elements ต่อเนื่องตาม representation ของ array จึงคำนวณตำแหน่งสมาชิกจาก base + index ได้ และ hardware prefetch/cache มักใช้ประโยชน์ได้ดี

## 8. Alignment and padding preview

CPU/ABI มี alignment requirements/preference บางชนิด struct จึงอาจมีช่องว่าง:

    struct Example {
        char c;
        int x;
    };

sizeof อาจมากกว่า 1 + 4

Chapter 126 จะวิเคราะห์เรื่องนี้อย่างเป็นระบบ

## 9. Cache hierarchy mental model

โดยทั่วไป CPU เข้าถึง register/cache เร็วกว่าการไป memory ไกลกว่า แต่ตัวเลข latency เปลี่ยนตาม hardware

สอง locality สำคัญ:

Temporal locality:
ข้อมูลที่เพิ่งใช้มีแนวโน้มถูกใช้อีก

Spatial locality:
ข้อมูลใกล้ตำแหน่งที่เพิ่งใช้มีแนวโน้มถูกใช้

Array traversal แบบเรียงติดกันจึงมักดีต่อ cache

## 10. Pointer chasing

Linked nodes อาจอยู่คนละตำแหน่ง:

    node A -> node B -> node C

การหาตำแหน่ง B ต้องอ่าน pointer จาก A ก่อน จึงเกิด dependency และ locality อาจแย่กว่า array traversal

นี่เป็นเหตุผลว่าทำไม Big-O เท่ากันไม่ได้หมายถึง performance เท่ากัน

## 11. Tools

AddressSanitizer:
จับ memory errors หลายชนิดขณะรัน

Valgrind:
instrument execution เพื่อตรวจ memory errors/leaks (มี overhead สูง)

GDB:
ดู address/pointer/state

perf:
จะใช้ใน chapters performance ภายหลัง

## 12. Chapter connection

Chapter 003 จะใช้ memory knowledge นี้สร้าง opaque Stack ADT: caller มองเห็น interface แต่ representation ที่มี data/size/capacity ถูกซ่อน

Chapter 006 จะกลับมาเจาะ static/dynamic arrays
Chapter 008 จะเห็นผลของ pointer chasing ใน linked list
Chapter 126–130 จะกลับมาลงลึก alignment/cache/false sharing/pointer chasing
