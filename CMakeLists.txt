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
add_library(ch003_int_stack 003-abstract-data-type/src/int_stack.c)
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
add_library(ch005_recursion 005-recursion-iteration/src/recursion_iteration.c)
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
add_library(ch006_int_vector 006-arrays/src/int_vector.c)
target_include_directories(ch006_int_vector PUBLIC 006-arrays/src)
add_executable(ch006_array_layout 006-arrays/examples/array_layout.c)
add_test(NAME ch006_array_layout_smoke COMMAND ch006_array_layout)
add_executable(ch006_vector_tests 006-arrays/tests/test_int_vector.c)
target_link_libraries(ch006_vector_tests PRIVATE ch006_int_vector)
add_test(NAME ch006_vector_tests COMMAND ch006_vector_tests)
add_executable(ch006_vector_benchmark 006-arrays/benchmarks/vector_operations.c)
target_link_libraries(ch006_vector_benchmark PRIVATE ch006_int_vector)

# Chapter 007
add_library(ch007_byte_string 007-strings/src/byte_string.c)
target_include_directories(ch007_byte_string PUBLIC 007-strings/src)
add_executable(ch007_c_string_layout 007-strings/examples/c_string_layout.c)
add_test(NAME ch007_c_string_layout_smoke COMMAND ch007_c_string_layout)
add_executable(ch007_string_tests 007-strings/tests/test_byte_string.c)
target_link_libraries(ch007_string_tests PRIVATE ch007_byte_string)
add_test(NAME ch007_string_tests COMMAND ch007_string_tests)
add_executable(ch007_string_benchmark 007-strings/benchmarks/string_building.c)
target_link_libraries(ch007_string_benchmark PRIVATE ch007_byte_string)

# Chapter 008
add_library(ch008_lists
    008-linked-lists/src/int_slist.c
    008-linked-lists/src/int_dlist.c
    008-linked-lists/src/int_clist.c
)
target_include_directories(ch008_lists PUBLIC 008-linked-lists/src)
add_executable(ch008_list_demo 008-linked-lists/examples/list_demo.c)
target_link_libraries(ch008_list_demo PRIVATE ch008_lists)
add_test(NAME ch008_list_demo_smoke COMMAND ch008_list_demo)
add_executable(ch008_list_tests 008-linked-lists/tests/test_linked_lists.c)
target_link_libraries(ch008_list_tests PRIVATE ch008_lists)
add_test(NAME ch008_list_tests COMMAND ch008_list_tests)
add_executable(ch008_list_vector_benchmark 008-linked-lists/benchmarks/list_vs_vector.c)
target_link_libraries(ch008_list_vector_benchmark PRIVATE ch008_lists ch006_int_vector)

# Chapter 009
add_library(ch009_stacks
    009-stack/src/array_stack.c
    009-stack/src/linked_stack.c
    009-stack/src/monotonic_stack.c
)
target_include_directories(ch009_stacks PUBLIC 009-stack/src)
add_executable(ch009_stack_demo 009-stack/examples/stack_demo.c)
target_link_libraries(ch009_stack_demo PRIVATE ch009_stacks)
add_test(NAME ch009_stack_demo_smoke COMMAND ch009_stack_demo)
add_executable(ch009_stack_tests 009-stack/tests/test_stacks.c)
target_link_libraries(ch009_stack_tests PRIVATE ch009_stacks)
add_test(NAME ch009_stack_tests COMMAND ch009_stack_tests)
add_executable(ch009_stack_benchmark 009-stack/benchmarks/stack_benchmark.c)
target_link_libraries(ch009_stack_benchmark PRIVATE ch009_stacks)

# Chapter 010
add_library(ch010_queues
    010-queue/src/circular_queue.c
    010-queue/src/linked_queue.c
)
target_include_directories(ch010_queues PUBLIC 010-queue/src)
add_executable(ch010_queue_demo 010-queue/examples/queue_demo.c)
target_link_libraries(ch010_queue_demo PRIVATE ch010_queues)
add_test(NAME ch010_queue_demo_smoke COMMAND ch010_queue_demo)
add_executable(ch010_queue_tests 010-queue/tests/test_queues.c)
target_link_libraries(ch010_queue_tests PRIVATE ch010_queues)
add_test(NAME ch010_queue_tests COMMAND ch010_queue_tests)
add_executable(ch010_queue_benchmark 010-queue/benchmarks/queue_benchmark.c)
target_link_libraries(ch010_queue_benchmark PRIVATE ch010_queues)

# Chapter 011
add_library(ch011_deque
    011-deque/src/int_deque.c
    011-deque/src/monotonic_queue.c
)
target_include_directories(ch011_deque PUBLIC 011-deque/src)
add_executable(ch011_deque_demo 011-deque/examples/deque_demo.c)
target_link_libraries(ch011_deque_demo PRIVATE ch011_deque)
add_test(NAME ch011_deque_demo_smoke COMMAND ch011_deque_demo)
add_executable(ch011_deque_tests 011-deque/tests/test_deque.c)
target_link_libraries(ch011_deque_tests PRIVATE ch011_deque)
add_test(NAME ch011_deque_tests COMMAND ch011_deque_tests)
add_executable(ch011_deque_benchmark 011-deque/benchmarks/deque_benchmark.c)
target_link_libraries(ch011_deque_benchmark PRIVATE ch011_deque)

# Chapter 012
add_library(ch012_priority_queue
    012-priority-queue/src/max_priority_queue.c
)
target_include_directories(ch012_priority_queue PUBLIC 012-priority-queue/src)
add_executable(ch012_priority_queue_demo 012-priority-queue/examples/priority_queue_demo.c)
target_link_libraries(ch012_priority_queue_demo PRIVATE ch012_priority_queue)
add_test(NAME ch012_priority_queue_demo_smoke COMMAND ch012_priority_queue_demo)
add_executable(ch012_priority_queue_tests 012-priority-queue/tests/test_priority_queue.c)
target_link_libraries(ch012_priority_queue_tests PRIVATE ch012_priority_queue)
add_test(NAME ch012_priority_queue_tests COMMAND ch012_priority_queue_tests)
add_executable(ch012_priority_queue_benchmark 012-priority-queue/benchmarks/priority_queue_benchmark.c)
target_link_libraries(ch012_priority_queue_benchmark PRIVATE ch012_priority_queue)


# Chapter 013
add_library(ch013_hashing
    013-hashing-fundamentals/src/hash_functions.c
)
target_include_directories(ch013_hashing PUBLIC 013-hashing-fundamentals/src)

add_executable(ch013_hash_demo 013-hashing-fundamentals/examples/hash_demo.c)
target_link_libraries(ch013_hash_demo PRIVATE ch013_hashing)
add_test(NAME ch013_hash_demo_smoke COMMAND ch013_hash_demo)

add_executable(ch013_hash_tests 013-hashing-fundamentals/tests/test_hash_functions.c)
target_link_libraries(ch013_hash_tests PRIVATE ch013_hashing)
add_test(NAME ch013_hash_tests COMMAND ch013_hash_tests)

add_executable(ch013_hash_distribution 013-hashing-fundamentals/benchmarks/distribution.c)
target_link_libraries(ch013_hash_distribution PRIVATE ch013_hashing)

# Chapter 014
add_library(ch014_hash_table
    014-hash-table/src/int_int_hash_table.c
)
target_include_directories(ch014_hash_table PUBLIC 014-hash-table/src)
target_link_libraries(ch014_hash_table PUBLIC ch013_hashing)

add_executable(ch014_hash_table_demo 014-hash-table/examples/hash_table_demo.c)
target_link_libraries(ch014_hash_table_demo PRIVATE ch014_hash_table)
add_test(NAME ch014_hash_table_demo_smoke COMMAND ch014_hash_table_demo)

add_executable(ch014_hash_table_tests 014-hash-table/tests/test_hash_table.c)
target_link_libraries(ch014_hash_table_tests PRIVATE ch014_hash_table)
add_test(NAME ch014_hash_table_tests COMMAND ch014_hash_table_tests)

add_executable(ch014_hash_table_benchmark 014-hash-table/benchmarks/hash_table_benchmark.c)
target_link_libraries(ch014_hash_table_benchmark PRIVATE ch014_hash_table)

# Chapter 015
add_library(ch015_hash_set_map
    015-hash-set-hash-map/src/int_hash_set.c
    015-hash-set-hash-map/src/string_int_hash_map.c
)
target_include_directories(ch015_hash_set_map PUBLIC 015-hash-set-hash-map/src)
target_link_libraries(ch015_hash_set_map PUBLIC ch014_hash_table ch013_hashing)

add_executable(ch015_hash_set_map_demo 015-hash-set-hash-map/examples/hash_set_map_demo.c)
target_link_libraries(ch015_hash_set_map_demo PRIVATE ch015_hash_set_map)
add_test(NAME ch015_hash_set_map_demo_smoke COMMAND ch015_hash_set_map_demo)

add_executable(ch015_hash_set_map_tests 015-hash-set-hash-map/tests/test_hash_set_map.c)
target_link_libraries(ch015_hash_set_map_tests PRIVATE ch015_hash_set_map)
add_test(NAME ch015_hash_set_map_tests COMMAND ch015_hash_set_map_tests)

add_executable(ch015_set_lookup_benchmark 015-hash-set-hash-map/benchmarks/set_lookup_benchmark.c)
target_link_libraries(ch015_set_lookup_benchmark PRIVATE ch015_hash_set_map)
