# Chapter 002 Theory Notes

## Object lifetime สำคัญกว่า address ที่ "ยังดูเดิม"

หลัง free(ptr) ตัวเลข address ใน ptr อาจยังไม่เปลี่ยน แต่สิทธิ์ในการใช้ storage นั้นผ่าน pointer เดิมสิ้นสุดตาม semantics ที่เกี่ยวข้อง

ดังนั้น:

    free(p);
    printf("%d", *p);

ผิดแม้บางครั้งจะพิมพ์ค่าดูถูกต้อง

## malloc failure

malloc สามารถคืน NULL ได้ production code ต้องกำหนด policy ว่าจะ propagate error, abort หรือ recovery อย่างไร

## realloc nuance

realloc อาจย้าย allocation ไป address ใหม่ pointer เก่าจึงใช้ต่อไม่ได้เมื่อ realloc สำเร็จและย้าย block

pattern ที่ปลอดภัยกว่า:

    int *tmp = realloc(data, new_bytes);
    if (tmp != NULL) {
        data = tmp;
    }

ถ้า assign ผล realloc ทับ data ทันทีแล้วได้ NULL เราอาจสูญเสีย reference ไป block เดิม

## Cache line preview

CPU cache ย้ายข้อมูลเป็น block/cache line ไม่ใช่ทีละ int แบบแนวคิด source code

ดังนั้น loop ที่อ่าน elements ต่อเนื่องมีโอกาสใช้ข้อมูลหลายตัวจาก cache line เดียว

## Row-major experiment

C array แบบสองมิติมี row-major layout

ถ้า iterate row ก่อน column เราเดิน memory ต่อเนื่องกว่า
ถ้า iterate column ก่อน row stride จะใหญ่ขึ้นและ cache behavior อาจแย่

benchmark ในบทนี้เป็น experiment ไม่ใช่กฎว่าผลเวลาต้องต่างเท่าใดทุกเครื่อง

## Stack overflow vs heap exhaustion

recursive depth มากอาจทำให้ call stack ใช้พื้นที่เกิน
dynamic allocation จำนวนมากอาจ fail หรือถูกระบบ terminate ภายใต้ memory pressure

สองปัญหานี้มาจาก resource/lifetime model ต่างกัน
