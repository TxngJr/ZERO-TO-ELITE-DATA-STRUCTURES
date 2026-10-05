# Complexity Derivation Cookbook

## Consecutive blocks

ถ้า A = Θ(f(n)) และ B = Θ(g(n)):

    total = Θ(f(n) + g(n))

dominant term มักครองผล เช่น Θ(n)+Θ(n²)=Θ(n²)

## Fixed nested loops

n outer × n inner = Θ(n²)

n outer × log n inner = Θ(n log n)

## Dependent loop

    Σ(i=1..n) i
    = n(n+1)/2
    = Θ(n²)

## Repeated multiplication/division

ถ้าคูณหรือหารค่าด้วย constant c>1 ทุก iteration มักได้ Θ(log n)

## Space

few scalar counters → auxiliary Θ(1)

recursion depth d กับ constant-size frame model → auxiliary Θ(d)

separate copy buffer n elements → auxiliary Θ(n)

## Amortized doubling

copy sizes:

    1,2,4,...,2^k

sum < 2n

บวก n writes ได้ total Θ(n) สำหรับ n pushes จึง Θ(1) amortized/push
