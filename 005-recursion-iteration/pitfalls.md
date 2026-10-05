# Pitfalls — Recursion & Iteration

1. missing base case
2. base case exists but progress never reaches it
3. wrong return composition during unwind
4. off-by-one binary-search interval
5. assuming recursive means exponential
6. assuming iterative is always faster
7. ignoring call-stack auxiliary space
8. assuming tail-call optimization is guaranteed
9. exponential repeated work such as naive Fibonacci
10. mutating shared state across recursive branches unexpectedly
11. deep recursion over adversarial/skewed structure
12. integer overflow hidden inside otherwise correct recursion

Debug tactic:
- print/inspect parameters by depth
- use GDB backtrace
- write base/progress/postcondition before changing code
