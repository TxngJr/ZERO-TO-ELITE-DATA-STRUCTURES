# Lab 007 — Strings as Memory

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Part A — C string layout

~~~bash
./build/ch007_c_string_layout
~~~

Observe:
- sizeof literal-backed local array
- strlen
- terminator address
- difference between embedded '\0' array and visible C-string length

## Part B — ByteString tests

~~~bash
./build/ch007_string_tests
~~~

Read tests for:
- append
- insert
- erase
- aliasing self-append
- randomized operations

## Part C — Sanitizers

~~~bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
~~~

## Part D — Quadratic construction experiment

Run:

~~~bash
./build/ch007_string_benchmark
~~~

Compare:
- naive repeated rebuild
- ByteString append

Explain why measured curve may be noisy but algorithmic copy counts differ.

## Part E — UTF-8 observation

Create a UTF-8 string containing Thai + English.
Print byte length using strlen.
Do not claim this equals human character count.
