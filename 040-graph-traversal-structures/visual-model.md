# Visual Model — Frontier State

Graph:

    0 -- 1 -- 3
    |    |
    2 -- 4

BFS from 0:

queue:
    [0]
    [1,2]
    [2,3,4]
    ...

depth:
    0:0
    1:1
    2:1
    3:2
    4:2

DFS instead uses a LIFO stack and may dive through one branch first.
