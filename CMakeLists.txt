cmake_minimum_required(VERSION 3.20)
project(zero_to_elite_data_structures LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DS_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(DS_ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

enable_testing()

# Chapter 001
add_executable(ch001_foundations 001-programming-foundations/examples/foundations.c)
add_test(NAME ch001_foundations_smoke COMMAND ch001_foundations)

add_executable(ch001_generics 001-programming-foundations/examples/generics.cpp)
add_test(NAME ch001_generics_smoke COMMAND ch001_generics)

# Chapter 002
add_executable(ch002_memory_walk 002-memory-fundamentals/examples/memory_walk.c)
add_test(NAME ch002_memory_walk_smoke COMMAND ch002_memory_walk)

add_executable(ch002_locality_benchmark 002-memory-fundamentals/benchmarks/locality.c)

# Chapter 003
add_library(ch003_int_stack
    003-abstract-data-type/src/int_stack.c
)
target_include_directories(ch003_int_stack PUBLIC 003-abstract-data-type/src)

add_executable(ch003_stack_tests 003-abstract-data-type/tests/test_int_stack.c)
target_link_libraries(ch003_stack_tests PRIVATE ch003_int_stack)
add_test(NAME ch003_stack_tests COMMAND ch003_stack_tests)

# Chapter 004
add_executable(ch004_operation_counts 004-complexity-analysis/examples/operation_counts.c)
add_executable(ch004_complexity_tests 004-complexity-analysis/tests/test_complexity.c)
add_test(NAME ch004_complexity_tests COMMAND ch004_complexity_tests)
add_executable(ch004_scaling_benchmark 004-complexity-analysis/benchmarks/scaling.c)

# Chapter 005
add_library(ch005_recursion
    005-recursion-iteration/src/recursion_iteration.c
)
target_include_directories(ch005_recursion PUBLIC 005-recursion-iteration/src)

add_executable(ch005_demo 005-recursion-iteration/examples/demo.c)
target_link_libraries(ch005_demo PRIVATE ch005_recursion)
add_test(NAME ch005_demo_smoke COMMAND ch005_demo)

add_executable(ch005_recursion_tests 005-recursion-iteration/tests/test_recursion_iteration.c)
target_link_libraries(ch005_recursion_tests PRIVATE ch005_recursion)
add_test(NAME ch005_recursion_tests COMMAND ch005_recursion_tests)

add_executable(ch005_recursion_benchmark 005-recursion-iteration/benchmarks/recursion_vs_iteration.c)
target_link_libraries(ch005_recursion_benchmark PRIVATE ch005_recursion)

# Chapter 006
add_library(ch006_int_vector
    006-arrays/src/int_vector.c
)
target_include_directories(ch006_int_vector PUBLIC 006-arrays/src)

add_executable(ch006_array_layout 006-arrays/examples/array_layout.c)
add_test(NAME ch006_array_layout_smoke COMMAND ch006_array_layout)

add_executable(ch006_vector_tests 006-arrays/tests/test_int_vector.c)
target_link_libraries(ch006_vector_tests PRIVATE ch006_int_vector)
add_test(NAME ch006_vector_tests COMMAND ch006_vector_tests)

add_executable(ch006_vector_benchmark 006-arrays/benchmarks/vector_operations.c)
target_link_libraries(ch006_vector_benchmark PRIVATE ch006_int_vector)
