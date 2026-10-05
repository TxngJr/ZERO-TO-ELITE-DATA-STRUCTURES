# Complexity — Linked Lists

Let n=size.

## Singly with head+tail

| Operation | Time |
|---|---:|
| push_front | Θ(1) |
| push_back | Θ(1) |
| pop_front | Θ(1) |
| pop_back | Θ(n) |
| get(i) | Θ(i) |
| insert(index) | Θ(n) worst |
| erase(index) | Θ(n) worst |
| validate | Θ(n) |

## Doubly with head+tail

| Operation | Time |
|---|---:|
| push_front/back | Θ(1) |
| pop_front/back | Θ(1) |
| get(i) | Θ(min(i,n-1-i)) |
| insert(index) | Θ(n) worst due lookup |
| erase(index) | Θ(n) worst due lookup |
| erase known node conceptually | Θ(1) relinking |
| validate | Θ(n) |

## Circular singly with tail

| Operation | Time |
|---|---:|
| head access | Θ(1) |
| push_back | Θ(1) |
| pop_front | Θ(1) |
| rotate_left | Θ(1) |
| get(i) | Θ(i) |

Space:
all list forms use Θ(n) nodes.
Singly metadata per node Θ(1) one link.
Doubly metadata per node Θ(1) two links.
