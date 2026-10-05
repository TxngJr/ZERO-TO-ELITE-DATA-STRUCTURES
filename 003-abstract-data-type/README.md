# Chapter 003 — Abstract Data Type (ADT)

## เป้าหมาย

บทนี้แยกคำที่มักถูกปนกัน: "Data Structure" และ "Abstract Data Type"

หลังจบบทนี้คุณควร:

- นิยาม ADT ด้วย values + operations + behavior ได้
- แยก interface ออกจาก implementation
- แยก representation ออกจาก observable behavior
- อธิบาย representation invariant
- ออกแบบ precondition/postcondition
- ใช้ opaque type ใน C เพื่อซ่อน representation
- implement Stack ADT ด้วย growable contiguous buffer
- เขียน unit tests จาก contract โดยไม่ผูกกับ internal representation

## 1. ปัญหา: ถ้า caller รู้ internals มากเกินไป

สมมติ stack เปิด fields:

    struct Stack {
        int *data;
        size_t size;
        size_t capacity;
    };

caller อาจทำ:

    stack.size = 999999;

โดยไม่ allocate storage รองรับ invariant พังทันที

Encapsulation จึงไม่ใช่เรื่อง "โค้ดสวย" อย่างเดียว แต่ลดจำนวน state ที่ผู้ใช้สามารถทำให้ผิดได้

## 2. ADT คืออะไร

Abstract Data Type กำหนด:

- เซตของค่าหรือ states เชิงนามธรรม
- operations ที่อนุญาต
- behavior/contract ของ operations

Stack ADT เชิงนามธรรม:

- empty stack
- push(x)
- pop()
- top()/peek()
- is_empty()
- size()

หลักสำคัญ: ADT ไม่บอกว่าต้องใช้ array หรือ linked list

## 3. Data Structure คือ representation

Stack ADT ทำได้หลายแบบ:

    Stack ADT
       |
       +--> array-backed stack
       |
       +--> linked-list-backed stack
       |
       +--> segmented stack

ADT ถาม "ผู้ใช้เห็น behavior อะไร"
Data Structure/implementation ถาม "เราจัด state ใน memory อย่างไรเพื่อให้ behavior นั้นเกิดขึ้น"

## 4. Interface vs implementation

ไฟล์ int_stack.h ในบทนี้เปิดเฉพาะ type handle และ functions
ไฟล์ int_stack.c เก็บ struct definition จริง

caller จึงไม่สามารถเขียน stack->size โดยตรง เพราะ type เป็น opaque

นี่เป็นตัวอย่าง abstraction boundary

## 5. Representation invariant

implementation ของเรารักษา:

- size <= capacity
- capacity == 0 iff data == NULL ใน state ที่ destroy/empty-initialized ตาม implementation นี้
- ถ้า size > 0, data ชี้ allocation ที่มีพื้นที่อย่างน้อย capacity ints
- logical stack elements อยู่ใน data[0..size-1]
- top อยู่ที่ data[size-1]

ถ้า invariant จริงก่อน operation และ operation ถูกต้อง เราต้องทำให้ invariant จริงหลัง operation

## 6. Contract

int_stack_init:
postcondition: stack อยู่ใน empty valid state

push(value):
ถ้าสำเร็จ size เพิ่ม 1 และ top เป็น value
ถ้า allocation failure ต้องไม่ทำให้ stack กลายเป็น state พัง

pop(out):
ถ้า stack ว่าง คืน false และไม่ลด size
ถ้าไม่ว่าง คืน element บนสุดแล้ว size ลด 1

peek(out):
ไม่แก้ state

destroy:
คืน owned memory และทำให้ handle กลับสู่ empty state ที่ destroy ซ้ำได้ตาม implementation นี้

## 7. Why opaque C type?

Header:

    typedef struct IntStack IntStack;

caller รู้ว่ามี type นี้ แต่ไม่รู้ fields

อย่างไรก็ตาม opaque pointer design แบบเต็มที่มักใช้ create/destroy ที่ allocate object เอง ส่วนตัวอย่างนี้เปิด struct size ไม่ได้จึงใช้ create/free API เพื่อรักษา opacity อย่างสมบูรณ์

ใน source ของบทนี้เราใช้:

- int_stack_create
- int_stack_free
- push/pop/peek/size/is_empty

## 8. Complexity preview

ยังไม่ formalize Big-O จน Chapter 004 แต่เราสังเกตได้:

- peek ดู index ล่าสุด ไม่ต้องเดินทุก element
- pop ลด size
- push ปกติเขียนตำแหน่งท้าย แต่บางครั้งต้อง grow buffer และ copy elements

นี่คือเหตุผลที่ dynamic-array push จะมี "occasionally expensive" operation และต่อไปจะนำไปสู่ amortized analysis

## 9. Testing from behavior

Test ไม่ควรรู้ว่า stack ใช้ array

bad test:
"capacity ต้องเป็น 8 หลัง push ครั้งแรก"

good test:
"push 1,2,3 แล้ว pop ต้องได้ 3,2,1"

เพราะ capacity เป็น implementation detail แต่ LIFO เป็น ADT contract

## 10. ADT invariants vs representation choice

ถ้าเปลี่ยน implementation เป็น linked list tests เชิง behavior ชุดเดิมควรยังใช้ได้

นี่คือประโยชน์สำคัญของ abstraction:
caller dependence อยู่ที่ contract ไม่ใช่ layout

## 11. Error policy

C ไม่มี exception มาตรฐานสำหรับ allocation failure เราเลือก bool-return API

production API อาจเลือก:
- error code
- errno-like channel
- abort
- allocator callback
- exception ใน C++
- Result ใน Rust

ไม่มีคำตอบเดียว สิ่งสำคัญคือต้องกำหนด contract

## 12. เมื่อไม่ควรสร้าง abstraction หนาเกินไป

Abstraction มี cost:
- API complexity
- indirection
- debugging boundary
- allocation strategy อาจถูกซ่อนจน tuning ยาก

แต่การเปิด internals โดยไม่มีเหตุผลก็เพิ่ม coupling

เราต้องเลือก abstraction boundary ตาม invariants และ users

## 13. Connection

Chapter 004 จะ formalize cost ของ operations
Chapter 006 จะสร้าง array/dynamic array อย่างเต็มรูปแบบ
Chapter 009 จะกลับมาสร้าง Stack หลาย representation และเปรียบเทียบ array stack vs linked stack
