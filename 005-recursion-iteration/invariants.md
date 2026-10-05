# Invariants and Correctness

## Factorial recursive

Precondition:
    0 <= n <= 20

Base:
    n<=1 returns 1

Progress:
    n decreases by 1

Postcondition:
    result = n!

## Factorial iterative loop invariant

Before each iteration for current i:

    result = product of integers in [2, i)

Initialization:
    i=2, empty product = 1

Maintenance:
    multiply by i then increment i

Termination:
    i=n+1, result=n!

## Binary search invariant

At loop/recursive entry:
if target exists in original searched range and has not been ruled out, a valid occurrence remains inside [lo,hi)

Each comparison removes a portion that cannot contain target under sorted-order assumption.

## GCD invariant

Euclid step preserves gcd:

    gcd(a,b)=gcd(b,a mod b)

Progress:
remainder is smaller than b when b>0
