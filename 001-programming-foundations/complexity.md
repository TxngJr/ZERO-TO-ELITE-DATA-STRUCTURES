# Complexity — Chapter 001

Formal asymptotic analysis is intentionally deferred to Chapter 004.

Still, build intuition:

- assigning one scalar is a bounded amount of work under the machine model used by the course
- a loop with n iterations performs work proportional to the number of iterations if each body has bounded work
- simple recursion may create one call per input step
- copying a large aggregate may cost more than passing an address/reference, but compiler optimization and ABI details matter

Do not label every statement O(1) mechanically yet. Chapter 004 will define the model, input size and asymptotic notation precisely.
