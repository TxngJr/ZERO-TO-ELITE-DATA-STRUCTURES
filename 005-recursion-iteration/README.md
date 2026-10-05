# Chapter 005 — Recursion & Iteration

## Goal

Recursion และ iteration เป็นสองวิธีหลักในการบอกคอมพิวเตอร์ให้ทำงานซ้ำ บทนี้ไม่ได้สอนแค่ syntax แต่สร้าง mental model ของ call stack, activation record, base case, progress, recursive structure และการแปลง recursive algorithm เป็น iterative algorithm

หลังจบบทนี้คุณควร:

- อธิบาย call stack แบบ simplified model ได้
- ระบุ base case และ recursive case
- พิสูจน์ termination จาก progress measure ง่าย ๆ
- trace recursive calls และ return order ได้
- แยก direct, indirect และ structural recursion
- เข้าใจ tail recursion โดยไม่สมมติว่า compiler ต้อง optimize
- เปรียบเทียบ recursion กับ iteration ด้าน readability, time และ auxiliary space
- แปลง recursion บางชนิดเป็น loop
- ใช้ explicit stack เมื่อ recursion ต้องเก็บหลาย pending states
- วิเคราะห์ recursion depth และ stack-overflow risk
- เขียน recursive/iterative binary search และตรวจผลเทียบกัน

## 1. Iteration

Iteration ใช้ loop และ explicit mutable state:

    result = 1
    for i = 2..n:
        result *= i

จุดสำคัญคือ loop invariant ที่บอกว่า state ปัจจุบันมีความหมายอะไร

สำหรับ factorial:
หลังจบ iteration ที่ i ค่า result = i!

## 2. Recursion

Recursive function แก้ปัญหาโดยเรียก problem ที่เล็กลง

factorial:

    fact(n) = 1                  if n <= 1
              n * fact(n - 1)   otherwise

ต้องมี:
1. base case
2. recursive case
3. progress ที่เข้าใกล้ base case

ถ้าขาดข้อใดข้อหนึ่ง อาจไม่ terminate หรือทำงานผิด

## 3. Call stack mental model

เมื่อ function A เรียก B, state ที่ A ต้องใช้ต่อจะถูกเก็บไว้ใน execution context ของ call

simplified trace:

    fact(4)
      waits for fact(3)
        waits for fact(2)
          waits for fact(1)
          returns 1
        returns 2
      returns 6
    returns 24

implementation จริงอาจใช้ registers, stack slots และ optimizer transformations ดังนั้นอย่าตีความ diagram เป็น exact machine layout

## 4. Stack frame / activation record

หนึ่ง active call อาจต้องเก็บ:
- return address
- parameters
- local values
- saved registers
- bookkeeping ตาม ABI

รายละเอียดจริงขึ้นกับ ABI/compiler/optimization

สิ่งที่ algorithm analysis สนใจคือแต่ละ active call ต้องใช้ทรัพยากรบางส่วน และ depth มากสามารถใช้ stack มาก

## 5. Termination proof

factorial ใช้ measure n

สำหรับ n>1:
recursive call ใช้ n-1

measure ลดลงและถูก bound ด้านล่างโดย base case จึง terminate

แนวคิดนี้สำคัญกว่าแค่ "มี if"

## 6. Direct vs Indirect Recursion

Direct:

    f -> f

Indirect:

    f -> g -> f

Indirect recursion อ่านยากกว่าเพราะ call cycle กระจายหลาย functions

## 7. Structural recursion

Data structure แบบ recursive เช่น tree:

    Tree = empty
         | Node(value, left_tree, right_tree)

algorithm จึงเขียน naturally:

    visit(node):
        if node == null: return
        visit(node.left)
        visit(node.right)

นี่เป็นเหตุผลที่ tree algorithms ใช้ recursion บ่อย

## 8. Tail recursion

Tail-recursive function ไม่มีงานสำคัญเหลือหลัง recursive call

ตัวอย่างเชิงแนวคิด:

    sum_tail(n, acc):
        if n == 0: return acc
        return sum_tail(n-1, acc+n)

บาง compiler/language/runtime สามารถทำ tail-call optimization แต่ C/C++ ไม่รับประกันทั่วไปว่าทุก tail call จะกลายเป็น constant-stack loop

อย่าใช้คำว่า tail recursion = O(1) stack โดยไม่ระบุ guarantee

## 9. Recursion to iteration

Simple recursion ที่มี state เดียวมักแปลงเป็น loop โดยนำ parameters มาเป็น loop variables

factorial:
recursive n ลดทีละ 1
iterative i เดินทีละ 1 หรือ n ลดทีละ 1

ถ้า recursion มีหลาย pending branches เช่น DFS tree/graph อาจต้องใช้ explicit Stack ADT เพื่อจำงานที่ยังไม่เสร็จ

## 10. Binary search

Recursive:

    search(lo, hi)
      mid
      recurse left or right half

Iterative:

    while lo < hi:
        mid
        shrink [lo,hi)

ทั้งคู่มี Θ(log n) comparisons ใน worst case บน sorted random-access array

แต่ recursion ใช้ call depth Θ(log n) ภายใต้ model ทั่วไป ส่วน iterative version ใช้ scalar auxiliary state Θ(1)

## 11. Recursion tree

Naive Fibonacci:

    fib(n)
      fib(n-1)
      fib(n-2)

เกิด overlapping subproblems และ call count เติบโตเร็ว

ปัญหาไม่ใช่ "recursion ช้าเสมอ" แต่ algorithm นี้ทำงานซ้ำจำนวนมาก

Memoization/dynamic programming เปลี่ยน structure ของ computation ได้ ซึ่งจะเรียนในบริบท algorithm ภายหลัง

## 12. Stack overflow

ไม่มี recursion depth universal ที่ปลอดภัย
ขึ้นกับ:
- thread stack size
- frame size
- compiler
- optimization
- OS/runtime

ดังนั้น input ที่ depth อาจใหญ่มากควรพิจารณา iterative/explicit-stack design

## 13. When recursion is a good fit

เหมาะเมื่อ:
- data definition เป็น recursive เช่น tree
- divide-and-conquer ชัดเจน
- depth ถูกควบคุม
- recursive version ทำ correctness reasoning ง่ายกว่า

ไม่เหมาะเมื่อ:
- depth อาจเป็น millions
- language/runtime stack limit เป็นข้อจำกัด
- function-call overhead สำคัญและ loop สื่อความชัดเท่ากัน
- recursion ซ่อน repeated work

## 14. Correctness pattern

Recursive proof มักใช้ induction

Base:
algorithm ถูกสำหรับ smallest problem

Inductive step:
สมมติ recursive call ถูกสำหรับ smaller problem แล้วพิสูจน์ว่า current call ใช้ผลนั้นสร้างคำตอบที่ถูก

Iterative proof มักใช้ loop invariant

นี่คือสะพานไป Chapters 138–140

## Files

- theory.md
- visual-model.md
- implementation.md
- complexity.md
- invariants.md
- pitfalls.md
- lab.md
- exercises.md
- quiz.md
- references.md
- src/recursion_iteration.c
- tests/test_recursion_iteration.c
- examples/demo.c
- benchmarks/recursion_vs_iteration.c
