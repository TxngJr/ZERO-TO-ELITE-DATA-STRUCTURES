# Lab 002 — Observe Memory

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Run memory walk

~~~bash
./build/ch002_memory_walk
~~~

จด addresses ที่เห็น อย่าคาดหวังว่าจะเหมือนกันทุก run เพราะ OS security/ASLR และ runtime mapping เปลี่ยนได้

## GDB

~~~bash
gdb ./build/ch002_memory_walk
~~~

ลอง:

    break main
    run
    info locals
    print &local
    next

## Sanitizer experiment

สร้างไฟล์ชั่วคราว bad.c:

    #include <stdlib.h>
    int main(void) {
        int *p = malloc(sizeof *p);
        free(p);
        return *p;
    }

compile:

~~~bash
gcc -g -O1 -fsanitize=address,undefined bad.c -o bad
./bad
~~~

อ่าน report แล้วตอบ:
- bug class คืออะไร
- allocation เกิดที่ไหน
- free เกิดที่ไหน
- invalid access เกิดที่ไหน

## Locality benchmark

~~~bash
./build/ch002_locality_benchmark
~~~

รันหลายครั้งและอย่าสรุปจาก run เดียว

## Valgrind

~~~bash
valgrind --leak-check=full ./build/ch002_memory_walk
~~~

เป้าหมายคือไม่มี definitely-lost block จากตัวอย่างที่ถูกต้อง
