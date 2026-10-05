# Implementation Notes — Chapter 001

This chapter is not implementing a container yet. Its implementation goal is to make language mechanisms observable.

## foundations.c

Demonstrates:
- struct representation unit
- recursive function
- pointer parameter
- dereference
- address printing
- deterministic return status for smoke testing

## generics.cpp

Demonstrates:
- C++ reference parameter
- class encapsulation
- template function
- template class
- const reference return
- move into owned state

## Design rule carried forward

Before implementing any data structure, separate:
1. public operation
2. state that operation may mutate
3. ownership of memory
4. invalid-input policy
5. invariant that must remain true
