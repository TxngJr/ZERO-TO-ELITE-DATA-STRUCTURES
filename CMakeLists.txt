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


# Chapter 016
add_library(ch016_tree_math
    016-trees-fundamentals/src/tree_math.c
)
target_include_directories(ch016_tree_math PUBLIC 016-trees-fundamentals/src)

add_executable(ch016_tree_math_demo 016-trees-fundamentals/examples/tree_math_demo.c)
target_link_libraries(ch016_tree_math_demo PRIVATE ch016_tree_math)
add_test(NAME ch016_tree_math_demo_smoke COMMAND ch016_tree_math_demo)

add_executable(ch016_tree_math_tests 016-trees-fundamentals/tests/test_tree_math.c)
target_link_libraries(ch016_tree_math_tests PRIVATE ch016_tree_math)
add_test(NAME ch016_tree_math_tests COMMAND ch016_tree_math_tests)

add_executable(ch016_tree_shape_cost 016-trees-fundamentals/benchmarks/tree_shape_cost.c)
target_link_libraries(ch016_tree_shape_cost PRIVATE ch016_tree_math)

# Chapter 017
add_library(ch017_binary_tree
    017-binary-tree/src/int_binary_tree.c
)
target_include_directories(ch017_binary_tree PUBLIC 017-binary-tree/src)

add_executable(ch017_binary_tree_demo 017-binary-tree/examples/binary_tree_demo.c)
target_link_libraries(ch017_binary_tree_demo PRIVATE ch017_binary_tree)
add_test(NAME ch017_binary_tree_demo_smoke COMMAND ch017_binary_tree_demo)

add_executable(ch017_binary_tree_tests 017-binary-tree/tests/test_binary_tree.c)
target_link_libraries(ch017_binary_tree_tests PRIVATE ch017_binary_tree)
add_test(NAME ch017_binary_tree_tests COMMAND ch017_binary_tree_tests)

add_executable(ch017_tree_shape_benchmark 017-binary-tree/benchmarks/tree_shape_benchmark.c)
target_link_libraries(ch017_tree_shape_benchmark PRIVATE ch017_binary_tree)

# Chapter 018
add_library(ch018_tree_traversal
    018-tree-traversal/src/tree_traversal.c
)
target_include_directories(ch018_tree_traversal PUBLIC 018-tree-traversal/src)
target_link_libraries(ch018_tree_traversal PUBLIC ch017_binary_tree)

add_executable(ch018_traversal_demo 018-tree-traversal/examples/traversal_demo.c)
target_link_libraries(ch018_traversal_demo PRIVATE ch018_tree_traversal)
add_test(NAME ch018_traversal_demo_smoke COMMAND ch018_traversal_demo)

add_executable(ch018_tree_traversal_tests 018-tree-traversal/tests/test_tree_traversal.c)
target_link_libraries(ch018_tree_traversal_tests PRIVATE ch018_tree_traversal)
add_test(NAME ch018_tree_traversal_tests COMMAND ch018_tree_traversal_tests)

add_executable(ch018_traversal_benchmark 018-tree-traversal/benchmarks/traversal_benchmark.c)
target_link_libraries(ch018_traversal_benchmark PRIVATE ch018_tree_traversal)


# Chapter 019
add_library(ch019_bst
    019-binary-search-tree/src/int_bst.c
)
target_include_directories(ch019_bst PUBLIC 019-binary-search-tree/src)

add_executable(ch019_bst_demo 019-binary-search-tree/examples/bst_demo.c)
target_link_libraries(ch019_bst_demo PRIVATE ch019_bst)
add_test(NAME ch019_bst_demo_smoke COMMAND ch019_bst_demo)

add_executable(ch019_bst_tests 019-binary-search-tree/tests/test_bst.c)
target_link_libraries(ch019_bst_tests PRIVATE ch019_bst)
add_test(NAME ch019_bst_tests COMMAND ch019_bst_tests)

add_executable(ch019_bst_shape_benchmark 019-binary-search-tree/benchmarks/bst_shape_benchmark.c)
target_link_libraries(ch019_bst_shape_benchmark PRIVATE ch019_bst)

# Chapter 020
add_library(ch020_rot_bst
    020-balanced-bst/src/rot_bst.c
)
target_include_directories(ch020_rot_bst PUBLIC 020-balanced-bst/src)

add_executable(ch020_rotation_demo 020-balanced-bst/examples/rotation_demo.c)
target_link_libraries(ch020_rotation_demo PRIVATE ch020_rot_bst)
add_test(NAME ch020_rotation_demo_smoke COMMAND ch020_rotation_demo)

add_executable(ch020_rotation_tests 020-balanced-bst/tests/test_rotations.c)
target_link_libraries(ch020_rotation_tests PRIVATE ch020_rot_bst)
add_test(NAME ch020_rotation_tests COMMAND ch020_rotation_tests)

add_executable(ch020_rotation_height_demo 020-balanced-bst/benchmarks/rotation_height_demo.c)
target_link_libraries(ch020_rotation_height_demo PRIVATE ch020_rot_bst)

# Chapter 021
add_library(ch021_avl
    021-avl-tree/src/int_avl.c
)
target_include_directories(ch021_avl PUBLIC 021-avl-tree/src)

add_executable(ch021_avl_demo 021-avl-tree/examples/avl_demo.c)
target_link_libraries(ch021_avl_demo PRIVATE ch021_avl)
add_test(NAME ch021_avl_demo_smoke COMMAND ch021_avl_demo)

add_executable(ch021_avl_tests 021-avl-tree/tests/test_avl.c)
target_link_libraries(ch021_avl_tests PRIVATE ch021_avl)
add_test(NAME ch021_avl_tests COMMAND ch021_avl_tests)

add_executable(ch021_avl_vs_bst 021-avl-tree/benchmarks/avl_vs_bst.c)
target_link_libraries(ch021_avl_vs_bst PRIVATE ch021_avl ch019_bst)


# Chapter 022
add_library(ch022_red_black_tree
    022-red-black-tree/src/int_red_black_tree.c
)
target_include_directories(ch022_red_black_tree PUBLIC 022-red-black-tree/src)

add_executable(ch022_red_black_demo 022-red-black-tree/examples/red_black_demo.c)
target_link_libraries(ch022_red_black_demo PRIVATE ch022_red_black_tree)
add_test(NAME ch022_red_black_demo_smoke COMMAND ch022_red_black_demo)

add_executable(ch022_red_black_tests 022-red-black-tree/tests/test_red_black_tree.c)
target_link_libraries(ch022_red_black_tests PRIVATE ch022_red_black_tree)
add_test(NAME ch022_red_black_tests COMMAND ch022_red_black_tests)

add_executable(ch022_red_black_vs_avl 022-red-black-tree/benchmarks/red_black_vs_avl.c)
target_link_libraries(ch022_red_black_vs_avl PRIVATE ch022_red_black_tree ch021_avl ch019_bst)

# Chapter 023
add_library(ch023_splay_tree
    023-splay-tree/src/int_splay_tree.c
)
target_include_directories(ch023_splay_tree PUBLIC 023-splay-tree/src)

add_executable(ch023_splay_demo 023-splay-tree/examples/splay_demo.c)
target_link_libraries(ch023_splay_demo PRIVATE ch023_splay_tree)
add_test(NAME ch023_splay_demo_smoke COMMAND ch023_splay_demo)

add_executable(ch023_splay_tests 023-splay-tree/tests/test_splay_tree.c)
target_link_libraries(ch023_splay_tests PRIVATE ch023_splay_tree)
add_test(NAME ch023_splay_tests COMMAND ch023_splay_tests)

add_executable(ch023_splay_hot_access 023-splay-tree/benchmarks/splay_hot_access.c)
target_link_libraries(ch023_splay_hot_access PRIVATE ch023_splay_tree ch019_bst)

# Chapter 024
add_library(ch024_treap
    024-treap/src/int_treap.c
)
target_include_directories(ch024_treap PUBLIC 024-treap/src)

add_executable(ch024_treap_demo 024-treap/examples/treap_demo.c)
target_link_libraries(ch024_treap_demo PRIVATE ch024_treap)
add_test(NAME ch024_treap_demo_smoke COMMAND ch024_treap_demo)

add_executable(ch024_treap_tests 024-treap/tests/test_treap.c)
target_link_libraries(ch024_treap_tests PRIVATE ch024_treap)
add_test(NAME ch024_treap_tests COMMAND ch024_treap_tests)

add_executable(ch024_treap_vs_bst 024-treap/benchmarks/treap_vs_bst.c)
target_link_libraries(ch024_treap_vs_bst PRIVATE ch024_treap ch019_bst)
