# Theory — Recursion & Iteration

## Recursive specification vs recursive implementation

Definition อาจ recursive แต่ implementation ไม่จำเป็นต้อง recursive

ตัวอย่าง factorial มี mathematical recurrence แต่เขียน loop ได้

ในทางกลับกัน tree representation อาจทำ recursive implementation อ่านง่ายมาก

## Induction correspondence

Recursive function:

    solve(n):
        if base(n): ...
        return combine(n, solve(smaller(n)))

proof:
- base case
- inductive hypothesis ว่า solve(smaller(n)) ถูก
- show combine สร้าง answer ของ n ถูก

## Loop invariant correspondence

Iterative:

    state = initial
    while condition:
        update state

proof:
1. initialization: invariant จริงก่อน loop
2. maintenance: iteration หนึ่งรอบรักษา invariant
3. termination: invariant + exit condition imply postcondition

## Recursion depth is not total work

Binary search:
- depth Θ(log n)
- one branch per level
- total work Θ(log n)

Tree traversal:
- depth อาจ Θ(h)
- total visits Θ(n)

ต้องแยก depth ออกจาก total number of calls

## Recursion on balanced vs skewed trees

Balanced tree:
    h ≈ log n

Skewed tree:
    h ≈ n

recursive traversal ทั้งคู่ visit n nodes แต่ maximum call-stack depth ต่างกัน

นี่จะสำคัญมากใน BST/balanced-tree chapters

## Explicit stack equivalence

Recursive DFS ใช้ runtime call stack เป็น implicit work stack
Iterative DFS ใช้ Stack ADT ที่โปรแกรมควบคุมเอง

explicit stack:
- ย้าย memory policy ออกจาก thread call stack
- สามารถ reserve/grow
- inspect/pause/serialize work ได้ง่ายกว่า
- แต่ code อาจซับซ้อนขึ้น
