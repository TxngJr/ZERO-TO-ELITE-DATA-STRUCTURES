# Visual Model — Stack

## Array stack

bottom
  |
  v
[10][20][30][ ][ ]
         ^
         top

push 40:

[10][20][30][40][ ]
             ^
             top

## Linked stack

top
 |
 v
[30| ] -> [20| ] -> [10|NULL]

push 40:

[40| ] -> [30| ] -> [20| ] -> [10|NULL]
 ^
 top

## Monotonic next greater

input = [2,1,5]

i=0:
stack indices [0]
values        [2]

i=1:
1 is not greater than 2
stack [0,1]
values [2,1]  decreasing

i=2:
5 > 1 -> resolve index 1
5 > 2 -> resolve index 0
push 2

stack [2]
