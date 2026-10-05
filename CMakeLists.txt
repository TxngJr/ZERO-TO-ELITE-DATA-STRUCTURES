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

add_executable(ch001_foundations 001-programming-foundations/examples/foundations.c)
add_test(NAME ch001_foundations_smoke COMMAND ch001_foundations)

add_executable(ch001_generics 001-programming-foundations/examples/generics.cpp)
add_test(NAME ch001_generics_smoke COMMAND ch001_generics)

add_executable(ch002_memory_walk 002-memory-fundamentals/examples/memory_walk.c)
add_test(NAME ch002_memory_walk_smoke COMMAND ch002_memory_walk)

add_executable(ch002_locality_benchmark 002-memory-fundamentals/benchmarks/locality.c)

add_library(ch003_int_stack
    003-abstract-data-type/src/int_stack.c
)
target_include_directories(ch003_int_stack PUBLIC 003-abstract-data-type/src)

add_executable(ch003_stack_tests 003-abstract-data-type/tests/test_int_stack.c)
target_link_libraries(ch003_stack_tests PRIVATE ch003_int_stack)
add_test(NAME ch003_stack_tests COMMAND ch003_stack_tests)
