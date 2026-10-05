# Theory — Complexity Analysis

## Function growth, not stopwatch time

Complexity มอง function:

    T : input size -> resource usage

เช่น T(n)=2n+4 primitive operations ภายใต้ model ที่กำหนด

wall-clock time มี machine-dependent effects เพิ่มเติม

## Proof pattern for O

พิสูจน์:

    3n + 7 ∈ O(n)

สำหรับ n >= 1:

    3n + 7 <= 3n + 7n = 10n

เลือก c=10 และ n0=1 ได้

## Proof pattern for Θ

ต้องมีทั้ง upper และ lower bound

upper:

    3n+7 <= 10n

lower:

    3n+7 >= 3n

ดังนั้น 3n+7 ∈ Θ(n)

## Log base

เมื่อฐาน a,b > 1 คงที่:

    log_a n = log_b n / log_b a

ต่างเพียง constant factor จึงเขียน Θ(log n) ได้

## Sums ที่ใช้บ่อย

Constant:

    1+1+... n terms = n

Arithmetic:

    1+2+...+n = n(n+1)/2 = Θ(n²)

Geometric:

    1+2+4+...+2^k = 2^(k+1)-1

Harmonic:

    1 + 1/2 + ... + 1/n = Θ(log n)

## Recurrence preview

Binary-search-like recurrence:

    T(n)=T(n/2)+Θ(1)

มีประมาณ log n levels

Merge-sort-like:

    T(n)=2T(n/2)+Θ(n)

มี recursion tree คนละรูป

## Expected complexity

Expected cost ต้องบอก randomness มาจากไหน เช่น randomized algorithm, randomized hash function หรือ input distribution

## Multiple parameters

Graph มักใช้ Θ(V+E)
storage algorithms อาจใช้ block size B
concurrency อาจพิจารณา thread count/contention

Cost model เปลี่ยนตาม domain
