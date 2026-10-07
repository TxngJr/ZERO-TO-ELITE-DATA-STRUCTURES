cmake_minimum_required(VERSION 3.20)
project(zero_to_elite_data_structures LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DS_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(DS_ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

if(DS_ENABLE_SANITIZERS AND DS_ENABLE_THREAD_SANITIZER)
    message(FATAL_ERROR "Address/Undefined sanitizers and ThreadSanitizer must use separate builds")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(DS_ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

if(DS_ENABLE_THREAD_SANITIZER AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
    add_link_options(-fsanitize=thread)
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


# Chapter 025
add_library(ch025_heap
    025-heap/src/int_min_heap.c
    025-heap/src/heap_sort.c
)
target_include_directories(ch025_heap PUBLIC 025-heap/src)

add_executable(ch025_heap_demo 025-heap/examples/heap_demo.c)
target_link_libraries(ch025_heap_demo PRIVATE ch025_heap)
add_test(NAME ch025_heap_demo_smoke COMMAND ch025_heap_demo)

add_executable(ch025_heap_tests 025-heap/tests/test_heap.c)
target_link_libraries(ch025_heap_tests PRIVATE ch025_heap)
add_test(NAME ch025_heap_tests COMMAND ch025_heap_tests)

add_executable(ch025_build_heap_benchmark 025-heap/benchmarks/build_heap_benchmark.c)
target_link_libraries(ch025_build_heap_benchmark PRIVATE ch025_heap)

# Chapter 026
add_library(ch026_dary_heap
    026-d-ary-heap/src/int_dary_heap.c
)
target_include_directories(ch026_dary_heap PUBLIC 026-d-ary-heap/src)

add_executable(ch026_dary_heap_demo 026-d-ary-heap/examples/dary_heap_demo.c)
target_link_libraries(ch026_dary_heap_demo PRIVATE ch026_dary_heap)
add_test(NAME ch026_dary_heap_demo_smoke COMMAND ch026_dary_heap_demo)

add_executable(ch026_dary_heap_tests 026-d-ary-heap/tests/test_dary_heap.c)
target_link_libraries(ch026_dary_heap_tests PRIVATE ch026_dary_heap)
add_test(NAME ch026_dary_heap_tests COMMAND ch026_dary_heap_tests)

add_executable(ch026_dary_benchmark 026-d-ary-heap/benchmarks/dary_benchmark.c)
target_link_libraries(ch026_dary_benchmark PRIVATE ch026_dary_heap)

# Chapter 027
add_library(ch027_binomial_heap
    027-binomial-heap/src/int_binomial_heap.c
)
target_include_directories(ch027_binomial_heap PUBLIC 027-binomial-heap/src)

add_executable(ch027_binomial_heap_demo 027-binomial-heap/examples/binomial_heap_demo.c)
target_link_libraries(ch027_binomial_heap_demo PRIVATE ch027_binomial_heap)
add_test(NAME ch027_binomial_heap_demo_smoke COMMAND ch027_binomial_heap_demo)

add_executable(ch027_binomial_heap_tests 027-binomial-heap/tests/test_binomial_heap.c)
target_link_libraries(ch027_binomial_heap_tests PRIVATE ch027_binomial_heap)
add_test(NAME ch027_binomial_heap_tests COMMAND ch027_binomial_heap_tests)

add_executable(ch027_meld_benchmark 027-binomial-heap/benchmarks/meld_benchmark.c)
target_link_libraries(ch027_meld_benchmark PRIVATE ch027_binomial_heap ch025_heap)


# Chapter 028
add_library(ch028_fibonacci_heap
    028-fibonacci-heap/src/int_fibonacci_heap.c
)
target_include_directories(ch028_fibonacci_heap PUBLIC 028-fibonacci-heap/src)

add_executable(ch028_fibonacci_heap_demo 028-fibonacci-heap/examples/fibonacci_heap_demo.c)
target_link_libraries(ch028_fibonacci_heap_demo PRIVATE ch028_fibonacci_heap)
add_test(NAME ch028_fibonacci_heap_demo_smoke COMMAND ch028_fibonacci_heap_demo)

add_executable(ch028_fibonacci_heap_tests 028-fibonacci-heap/tests/test_fibonacci_heap.c)
target_link_libraries(ch028_fibonacci_heap_tests PRIVATE ch028_fibonacci_heap)
add_test(NAME ch028_fibonacci_heap_tests COMMAND ch028_fibonacci_heap_tests)

add_executable(ch028_decrease_key_benchmark 028-fibonacci-heap/benchmarks/decrease_key_benchmark.c)
target_link_libraries(ch028_decrease_key_benchmark PRIVATE ch028_fibonacci_heap ch027_binomial_heap)

# Chapter 029
add_library(ch029_byte_trie
    029-trie-prefix-tree/src/byte_trie.c
)
target_include_directories(ch029_byte_trie PUBLIC 029-trie-prefix-tree/src)

add_executable(ch029_trie_demo 029-trie-prefix-tree/examples/trie_demo.c)
target_link_libraries(ch029_trie_demo PRIVATE ch029_byte_trie)
add_test(NAME ch029_trie_demo_smoke COMMAND ch029_trie_demo)

add_executable(ch029_trie_tests 029-trie-prefix-tree/tests/test_byte_trie.c)
target_link_libraries(ch029_trie_tests PRIVATE ch029_byte_trie)
add_test(NAME ch029_trie_tests COMMAND ch029_trie_tests)

add_executable(ch029_prefix_benchmark 029-trie-prefix-tree/benchmarks/prefix_benchmark.c)
target_link_libraries(ch029_prefix_benchmark PRIVATE ch029_byte_trie)

# Chapter 030
add_library(ch030_radix_tree
    030-radix-patricia-trie/src/byte_radix_tree.c
)
target_include_directories(ch030_radix_tree PUBLIC 030-radix-patricia-trie/src)

add_executable(ch030_radix_demo 030-radix-patricia-trie/examples/radix_demo.c)
target_link_libraries(ch030_radix_demo PRIVATE ch030_radix_tree)
add_test(NAME ch030_radix_demo_smoke COMMAND ch030_radix_demo)

add_executable(ch030_radix_tests 030-radix-patricia-trie/tests/test_radix_tree.c)
target_link_libraries(ch030_radix_tests PRIVATE ch030_radix_tree)
add_test(NAME ch030_radix_tests COMMAND ch030_radix_tests)

add_executable(ch030_compression_benchmark 030-radix-patricia-trie/benchmarks/compression_benchmark.c)
target_link_libraries(ch030_compression_benchmark PRIVATE ch030_radix_tree ch029_byte_trie)


# Chapter 031
add_library(ch031_tst
    031-ternary-search-tree/src/byte_tst.c
)
target_include_directories(ch031_tst PUBLIC 031-ternary-search-tree/src)

add_executable(ch031_tst_demo 031-ternary-search-tree/examples/tst_demo.c)
target_link_libraries(ch031_tst_demo PRIVATE ch031_tst)
add_test(NAME ch031_tst_demo_smoke COMMAND ch031_tst_demo)

add_executable(ch031_tst_tests 031-ternary-search-tree/tests/test_tst.c)
target_link_libraries(ch031_tst_tests PRIVATE ch031_tst)
add_test(NAME ch031_tst_tests COMMAND ch031_tst_tests)

add_executable(ch031_tst_lookup_benchmark 031-ternary-search-tree/benchmarks/tst_lookup_benchmark.c)
target_link_libraries(ch031_tst_lookup_benchmark PRIVATE ch031_tst ch029_byte_trie ch030_radix_tree)

# Chapter 032
add_library(ch032_btree
    032-b-tree/src/int_btree.c
)
target_include_directories(ch032_btree PUBLIC 032-b-tree/src)

add_executable(ch032_btree_demo 032-b-tree/examples/btree_demo.c)
target_link_libraries(ch032_btree_demo PRIVATE ch032_btree)
add_test(NAME ch032_btree_demo_smoke COMMAND ch032_btree_demo)

add_executable(ch032_btree_tests 032-b-tree/tests/test_btree.c)
target_link_libraries(ch032_btree_tests PRIVATE ch032_btree)
add_test(NAME ch032_btree_tests COMMAND ch032_btree_tests)

add_executable(ch032_fanout_benchmark 032-b-tree/benchmarks/fanout_benchmark.c)
target_link_libraries(ch032_fanout_benchmark PRIVATE ch032_btree)

# Chapter 033
add_library(ch033_bplus_tree
    033-b-plus-tree/src/int_bplus_tree.c
)
target_include_directories(ch033_bplus_tree PUBLIC 033-b-plus-tree/src)

add_executable(ch033_bplus_demo 033-b-plus-tree/examples/bplus_demo.c)
target_link_libraries(ch033_bplus_demo PRIVATE ch033_bplus_tree)
add_test(NAME ch033_bplus_demo_smoke COMMAND ch033_bplus_demo)

add_executable(ch033_bplus_tests 033-b-plus-tree/tests/test_bplus_tree.c)
target_link_libraries(ch033_bplus_tests PRIVATE ch033_bplus_tree)
add_test(NAME ch033_bplus_tests COMMAND ch033_bplus_tests)

add_executable(ch033_range_scan_benchmark 033-b-plus-tree/benchmarks/range_scan_benchmark.c)
target_link_libraries(ch033_range_scan_benchmark PRIVATE ch033_bplus_tree ch032_btree)


# Chapter 034
add_library(ch034_bstar_tree
    034-b-star-tree/src/int_bstar_tree.c
)
target_include_directories(ch034_bstar_tree PUBLIC 034-b-star-tree/src)

add_executable(ch034_bstar_demo 034-b-star-tree/examples/bstar_demo.c)
target_link_libraries(ch034_bstar_demo PRIVATE ch034_bstar_tree)
add_test(NAME ch034_bstar_demo_smoke COMMAND ch034_bstar_demo)

add_executable(ch034_bstar_tests 034-b-star-tree/tests/test_bstar_tree.c)
target_link_libraries(ch034_bstar_tests PRIVATE ch034_bstar_tree)
add_test(NAME ch034_bstar_tests COMMAND ch034_bstar_tests)

add_executable(ch034_occupancy_benchmark 034-b-star-tree/benchmarks/occupancy_benchmark.c)
target_link_libraries(ch034_occupancy_benchmark PRIVATE ch034_bstar_tree ch032_btree)

# Chapter 035
add_library(ch035_lsm_tree
    035-lsm-tree/src/int_lsm_tree.c
)
target_include_directories(ch035_lsm_tree PUBLIC 035-lsm-tree/src)

add_executable(ch035_lsm_demo 035-lsm-tree/examples/lsm_demo.c)
target_link_libraries(ch035_lsm_demo PRIVATE ch035_lsm_tree)
add_test(NAME ch035_lsm_demo_smoke COMMAND ch035_lsm_demo)

add_executable(ch035_lsm_tests 035-lsm-tree/tests/test_lsm_tree.c)
target_link_libraries(ch035_lsm_tests PRIVATE ch035_lsm_tree)
add_test(NAME ch035_lsm_tests COMMAND ch035_lsm_tests)

add_executable(ch035_read_amplification_benchmark 035-lsm-tree/benchmarks/read_amplification_benchmark.c)
target_link_libraries(ch035_read_amplification_benchmark PRIVATE ch035_lsm_tree)

# Chapter 036
add_library(ch036_skip_list
    036-skip-list/src/int_skip_list.c
)
target_include_directories(ch036_skip_list PUBLIC 036-skip-list/src)

add_executable(ch036_skip_list_demo 036-skip-list/examples/skip_list_demo.c)
target_link_libraries(ch036_skip_list_demo PRIVATE ch036_skip_list)
add_test(NAME ch036_skip_list_demo_smoke COMMAND ch036_skip_list_demo)

add_executable(ch036_skip_list_tests 036-skip-list/tests/test_skip_list.c)
target_link_libraries(ch036_skip_list_tests PRIVATE ch036_skip_list)
add_test(NAME ch036_skip_list_tests COMMAND ch036_skip_list_tests)

add_executable(ch036_skip_list_benchmark 036-skip-list/benchmarks/skip_list_benchmark.c)
target_link_libraries(ch036_skip_list_benchmark PRIVATE ch036_skip_list ch032_btree)


# Chapter 037
add_library(ch037_dsu
    037-disjoint-set-union-find/src/int_dsu.c
)
target_include_directories(ch037_dsu PUBLIC 037-disjoint-set-union-find/src)

add_executable(ch037_dsu_demo 037-disjoint-set-union-find/examples/dsu_demo.c)
target_link_libraries(ch037_dsu_demo PRIVATE ch037_dsu)
add_test(NAME ch037_dsu_demo_smoke COMMAND ch037_dsu_demo)

add_executable(ch037_dsu_tests 037-disjoint-set-union-find/tests/test_dsu.c)
target_link_libraries(ch037_dsu_tests PRIVATE ch037_dsu)
add_test(NAME ch037_dsu_tests COMMAND ch037_dsu_tests)

add_executable(ch037_dsu_benchmark 037-disjoint-set-union-find/benchmarks/dsu_benchmark.c)
target_link_libraries(ch037_dsu_benchmark PRIVATE ch037_dsu)

# Chapter 038
add_library(ch038_graph
    038-graphs-fundamentals/src/int_graph.c
)
target_include_directories(ch038_graph PUBLIC 038-graphs-fundamentals/src)

add_executable(ch038_graph_demo 038-graphs-fundamentals/examples/graph_demo.c)
target_link_libraries(ch038_graph_demo PRIVATE ch038_graph)
add_test(NAME ch038_graph_demo_smoke COMMAND ch038_graph_demo)

add_executable(ch038_graph_tests 038-graphs-fundamentals/tests/test_graph.c)
target_link_libraries(ch038_graph_tests PRIVATE ch038_graph)
add_test(NAME ch038_graph_tests COMMAND ch038_graph_tests)

add_executable(ch038_edge_lookup_benchmark 038-graphs-fundamentals/benchmarks/edge_lookup_benchmark.c)
target_link_libraries(ch038_edge_lookup_benchmark PRIVATE ch038_graph)

# Chapter 039
add_library(ch039_graph_repr
    039-graph-representations/src/graph_repr.c
)
target_include_directories(ch039_graph_repr PUBLIC 039-graph-representations/src)

add_executable(ch039_graph_repr_demo 039-graph-representations/examples/representations_demo.c)
target_link_libraries(ch039_graph_repr_demo PRIVATE ch039_graph_repr)
add_test(NAME ch039_graph_repr_demo_smoke COMMAND ch039_graph_repr_demo)

add_executable(ch039_graph_repr_tests 039-graph-representations/tests/test_graph_repr.c)
target_link_libraries(ch039_graph_repr_tests PRIVATE ch039_graph_repr)
add_test(NAME ch039_graph_repr_tests COMMAND ch039_graph_repr_tests)

add_executable(ch039_sparse_dense_benchmark 039-graph-representations/benchmarks/sparse_dense_benchmark.c)
target_link_libraries(ch039_sparse_dense_benchmark PRIVATE ch039_graph_repr)


# Chapter 040
add_library(ch040_graph_traversal
    040-graph-traversal-structures/src/graph_traversal.c
)
target_include_directories(ch040_graph_traversal PUBLIC 040-graph-traversal-structures/src)
target_link_libraries(ch040_graph_traversal PUBLIC ch039_graph_repr)

add_executable(ch040_graph_traversal_demo 040-graph-traversal-structures/examples/traversal_demo.c)
target_link_libraries(ch040_graph_traversal_demo PRIVATE ch040_graph_traversal)
add_test(NAME ch040_graph_traversal_demo_smoke COMMAND ch040_graph_traversal_demo)

add_executable(ch040_graph_traversal_tests 040-graph-traversal-structures/tests/test_graph_traversal.c)
target_link_libraries(ch040_graph_traversal_tests PRIVATE ch040_graph_traversal)
add_test(NAME ch040_graph_traversal_tests COMMAND ch040_graph_traversal_tests)

add_executable(ch040_traversal_repr_benchmark 040-graph-traversal-structures/benchmarks/traversal_repr_benchmark.c)
target_link_libraries(ch040_traversal_repr_benchmark PRIVATE ch040_graph_traversal)

# Chapter 041
add_library(ch041_dag
    041-dag-data-structures/src/int_dag.c
)
target_include_directories(ch041_dag PUBLIC 041-dag-data-structures/src)

add_executable(ch041_dag_demo 041-dag-data-structures/examples/dag_demo.c)
target_link_libraries(ch041_dag_demo PRIVATE ch041_dag)
add_test(NAME ch041_dag_demo_smoke COMMAND ch041_dag_demo)

add_executable(ch041_dag_tests 041-dag-data-structures/tests/test_dag.c)
target_link_libraries(ch041_dag_tests PRIVATE ch041_dag)
add_test(NAME ch041_dag_tests COMMAND ch041_dag_tests)

add_executable(ch041_topological_benchmark 041-dag-data-structures/benchmarks/topological_benchmark.c)
target_link_libraries(ch041_topological_benchmark PRIVATE ch041_dag)

# Chapter 042
add_library(ch042_adaptive_graph
    042-sparse-vs-dense-graph-representation/src/adaptive_graph.c
)
target_include_directories(ch042_adaptive_graph PUBLIC 042-sparse-vs-dense-graph-representation/src)
target_link_libraries(ch042_adaptive_graph PUBLIC ch039_graph_repr)

add_executable(ch042_adaptive_demo 042-sparse-vs-dense-graph-representation/examples/adaptive_demo.c)
target_link_libraries(ch042_adaptive_demo PRIVATE ch042_adaptive_graph)
add_test(NAME ch042_adaptive_demo_smoke COMMAND ch042_adaptive_demo)

add_executable(ch042_adaptive_graph_tests 042-sparse-vs-dense-graph-representation/tests/test_adaptive_graph.c)
target_link_libraries(ch042_adaptive_graph_tests PRIVATE ch042_adaptive_graph)
add_test(NAME ch042_adaptive_graph_tests COMMAND ch042_adaptive_graph_tests)

add_executable(ch042_adaptive_density_benchmark 042-sparse-vs-dense-graph-representation/benchmarks/adaptive_density_benchmark.c)
target_link_libraries(ch042_adaptive_density_benchmark PRIVATE ch042_adaptive_graph)


# Chapter 043
add_library(ch043_segment_tree
    043-segment-tree/src/int_segment_tree.c
)
target_include_directories(ch043_segment_tree PUBLIC 043-segment-tree/src)

add_executable(ch043_segment_tree_demo 043-segment-tree/examples/segment_tree_demo.c)
target_link_libraries(ch043_segment_tree_demo PRIVATE ch043_segment_tree)
add_test(NAME ch043_segment_tree_demo_smoke COMMAND ch043_segment_tree_demo)

add_executable(ch043_segment_tree_tests 043-segment-tree/tests/test_segment_tree.c)
target_link_libraries(ch043_segment_tree_tests PRIVATE ch043_segment_tree)
add_test(NAME ch043_segment_tree_tests COMMAND ch043_segment_tree_tests)

add_executable(ch043_segment_tree_benchmark 043-segment-tree/benchmarks/segment_tree_benchmark.c)
target_link_libraries(ch043_segment_tree_benchmark PRIVATE ch043_segment_tree)

# Chapter 044
add_library(ch044_lazy_segment_tree
    044-lazy-propagation-segment-tree/src/lazy_segment_tree.c
)
target_include_directories(ch044_lazy_segment_tree PUBLIC 044-lazy-propagation-segment-tree/src)

add_executable(ch044_lazy_demo 044-lazy-propagation-segment-tree/examples/lazy_demo.c)
target_link_libraries(ch044_lazy_demo PRIVATE ch044_lazy_segment_tree)
add_test(NAME ch044_lazy_demo_smoke COMMAND ch044_lazy_demo)

add_executable(ch044_lazy_segment_tree_tests 044-lazy-propagation-segment-tree/tests/test_lazy_segment_tree.c)
target_link_libraries(ch044_lazy_segment_tree_tests PRIVATE ch044_lazy_segment_tree)
add_test(NAME ch044_lazy_segment_tree_tests COMMAND ch044_lazy_segment_tree_tests)

add_executable(ch044_lazy_vs_naive 044-lazy-propagation-segment-tree/benchmarks/lazy_vs_naive.c)
target_link_libraries(ch044_lazy_vs_naive PRIVATE ch044_lazy_segment_tree)

# Chapter 045
add_library(ch045_dynamic_segment_tree
    045-dynamic-segment-tree/src/dynamic_segment_tree.c
)
target_include_directories(ch045_dynamic_segment_tree PUBLIC 045-dynamic-segment-tree/src)

add_executable(ch045_dynamic_demo 045-dynamic-segment-tree/examples/dynamic_demo.c)
target_link_libraries(ch045_dynamic_demo PRIVATE ch045_dynamic_segment_tree)
add_test(NAME ch045_dynamic_demo_smoke COMMAND ch045_dynamic_demo)

add_executable(ch045_dynamic_segment_tree_tests 045-dynamic-segment-tree/tests/test_dynamic_segment_tree.c)
target_link_libraries(ch045_dynamic_segment_tree_tests PRIVATE ch045_dynamic_segment_tree)
add_test(NAME ch045_dynamic_segment_tree_tests COMMAND ch045_dynamic_segment_tree_tests)

add_executable(ch045_sparse_universe_benchmark 045-dynamic-segment-tree/benchmarks/sparse_universe_benchmark.c)
target_link_libraries(ch045_sparse_universe_benchmark PRIVATE ch045_dynamic_segment_tree)


# Chapter 046
add_library(ch046_persistent_segment_tree
    046-persistent-segment-tree/src/persistent_segment_tree.c
)
target_include_directories(ch046_persistent_segment_tree PUBLIC 046-persistent-segment-tree/src)

add_executable(ch046_persistent_demo 046-persistent-segment-tree/examples/persistent_demo.c)
target_link_libraries(ch046_persistent_demo PRIVATE ch046_persistent_segment_tree)
add_test(NAME ch046_persistent_demo_smoke COMMAND ch046_persistent_demo)

add_executable(ch046_persistent_segment_tree_tests 046-persistent-segment-tree/tests/test_persistent_segment_tree.c)
target_link_libraries(ch046_persistent_segment_tree_tests PRIVATE ch046_persistent_segment_tree)
add_test(NAME ch046_persistent_segment_tree_tests COMMAND ch046_persistent_segment_tree_tests)

add_executable(ch046_version_memory_benchmark 046-persistent-segment-tree/benchmarks/version_memory_benchmark.c)
target_link_libraries(ch046_version_memory_benchmark PRIVATE ch046_persistent_segment_tree)

# Chapter 047
add_library(ch047_fenwick_tree
    047-fenwick-tree/src/int_fenwick_tree.c
)
target_include_directories(ch047_fenwick_tree PUBLIC 047-fenwick-tree/src)

add_executable(ch047_fenwick_demo 047-fenwick-tree/examples/fenwick_demo.c)
target_link_libraries(ch047_fenwick_demo PRIVATE ch047_fenwick_tree)
add_test(NAME ch047_fenwick_demo_smoke COMMAND ch047_fenwick_demo)

add_executable(ch047_fenwick_tree_tests 047-fenwick-tree/tests/test_fenwick_tree.c)
target_link_libraries(ch047_fenwick_tree_tests PRIVATE ch047_fenwick_tree)
add_test(NAME ch047_fenwick_tree_tests COMMAND ch047_fenwick_tree_tests)

add_executable(ch047_fenwick_benchmark 047-fenwick-tree/benchmarks/fenwick_benchmark.c)
target_link_libraries(ch047_fenwick_benchmark PRIVATE ch047_fenwick_tree)

# Chapter 048
add_library(ch048_sparse_table
    048-sparse-table/src/int_sparse_table.c
)
target_include_directories(ch048_sparse_table PUBLIC 048-sparse-table/src)

add_executable(ch048_sparse_table_demo 048-sparse-table/examples/sparse_table_demo.c)
target_link_libraries(ch048_sparse_table_demo PRIVATE ch048_sparse_table)
add_test(NAME ch048_sparse_table_demo_smoke COMMAND ch048_sparse_table_demo)

add_executable(ch048_sparse_table_tests 048-sparse-table/tests/test_sparse_table.c)
target_link_libraries(ch048_sparse_table_tests PRIVATE ch048_sparse_table)
add_test(NAME ch048_sparse_table_tests COMMAND ch048_sparse_table_tests)

add_executable(ch048_sparse_table_benchmark 048-sparse-table/benchmarks/sparse_table_benchmark.c)
target_link_libraries(ch048_sparse_table_benchmark PRIVATE ch048_sparse_table)


# Chapter 049
add_library(ch049_interval_tree
    049-interval-tree/src/int_interval_tree.c
)
target_include_directories(ch049_interval_tree PUBLIC 049-interval-tree/src)

add_executable(ch049_interval_tree_demo 049-interval-tree/examples/interval_tree_demo.c)
target_link_libraries(ch049_interval_tree_demo PRIVATE ch049_interval_tree)
add_test(NAME ch049_interval_tree_demo_smoke COMMAND ch049_interval_tree_demo)

add_executable(ch049_interval_tree_tests 049-interval-tree/tests/test_interval_tree.c)
target_link_libraries(ch049_interval_tree_tests PRIVATE ch049_interval_tree)
add_test(NAME ch049_interval_tree_tests COMMAND ch049_interval_tree_tests)

add_executable(ch049_overlap_benchmark 049-interval-tree/benchmarks/overlap_benchmark.c)
target_link_libraries(ch049_overlap_benchmark PRIVATE ch049_interval_tree)

# Chapter 050
add_library(ch050_interval_heap
    050-interval-heap/src/int_interval_heap.c
)
target_include_directories(ch050_interval_heap PUBLIC 050-interval-heap/src)

add_executable(ch050_interval_heap_demo 050-interval-heap/examples/interval_heap_demo.c)
target_link_libraries(ch050_interval_heap_demo PRIVATE ch050_interval_heap)
add_test(NAME ch050_interval_heap_demo_smoke COMMAND ch050_interval_heap_demo)

add_executable(ch050_interval_heap_tests 050-interval-heap/tests/test_interval_heap.c)
target_link_libraries(ch050_interval_heap_tests PRIVATE ch050_interval_heap)
add_test(NAME ch050_interval_heap_tests COMMAND ch050_interval_heap_tests)

add_executable(ch050_depq_benchmark 050-interval-heap/benchmarks/depq_benchmark.c)
target_link_libraries(ch050_depq_benchmark PRIVATE ch050_interval_heap)

# Chapter 051
add_library(ch051_range_tree
    051-range-tree/src/int_range_tree.c
)
target_include_directories(ch051_range_tree PUBLIC 051-range-tree/src)

add_executable(ch051_range_tree_demo 051-range-tree/examples/range_tree_demo.c)
target_link_libraries(ch051_range_tree_demo PRIVATE ch051_range_tree)
add_test(NAME ch051_range_tree_demo_smoke COMMAND ch051_range_tree_demo)

add_executable(ch051_range_tree_tests 051-range-tree/tests/test_range_tree.c)
target_link_libraries(ch051_range_tree_tests PRIVATE ch051_range_tree)
add_test(NAME ch051_range_tree_tests COMMAND ch051_range_tree_tests)

add_executable(ch051_range_query_benchmark 051-range-tree/benchmarks/range_query_benchmark.c)
target_link_libraries(ch051_range_query_benchmark PRIVATE ch051_range_tree)


# Chapter 052
add_library(ch052_order_stat_tree
    052-order-statistic-tree/src/int_order_stat_tree.c
)
target_include_directories(ch052_order_stat_tree PUBLIC 052-order-statistic-tree/src)

add_executable(ch052_order_stat_demo 052-order-statistic-tree/examples/order_stat_demo.c)
target_link_libraries(ch052_order_stat_demo PRIVATE ch052_order_stat_tree)
add_test(NAME ch052_order_stat_demo_smoke COMMAND ch052_order_stat_demo)

add_executable(ch052_order_stat_tree_tests 052-order-statistic-tree/tests/test_order_stat_tree.c)
target_link_libraries(ch052_order_stat_tree_tests PRIVATE ch052_order_stat_tree)
add_test(NAME ch052_order_stat_tree_tests COMMAND ch052_order_stat_tree_tests)

add_executable(ch052_rank_select_benchmark 052-order-statistic-tree/benchmarks/rank_select_benchmark.c)
target_link_libraries(ch052_rank_select_benchmark PRIVATE ch052_order_stat_tree)

# Chapter 053
add_library(ch053_cartesian_tree
    053-cartesian-tree/src/int_cartesian_tree.c
)
target_include_directories(ch053_cartesian_tree PUBLIC 053-cartesian-tree/src)

add_executable(ch053_cartesian_demo 053-cartesian-tree/examples/cartesian_demo.c)
target_link_libraries(ch053_cartesian_demo PRIVATE ch053_cartesian_tree)
add_test(NAME ch053_cartesian_demo_smoke COMMAND ch053_cartesian_demo)

add_executable(ch053_cartesian_tree_tests 053-cartesian-tree/tests/test_cartesian_tree.c)
target_link_libraries(ch053_cartesian_tree_tests PRIVATE ch053_cartesian_tree)
add_test(NAME ch053_cartesian_tree_tests COMMAND ch053_cartesian_tree_tests)

add_executable(ch053_cartesian_shape_benchmark 053-cartesian-tree/benchmarks/cartesian_shape_benchmark.c)
target_link_libraries(ch053_cartesian_shape_benchmark PRIVATE ch053_cartesian_tree)

# Chapter 054
add_library(ch054_kd_tree
    054-kd-tree/src/int_kd_tree.c
)
target_include_directories(ch054_kd_tree PUBLIC 054-kd-tree/src)

add_executable(ch054_kd_tree_demo 054-kd-tree/examples/kd_tree_demo.c)
target_link_libraries(ch054_kd_tree_demo PRIVATE ch054_kd_tree)
add_test(NAME ch054_kd_tree_demo_smoke COMMAND ch054_kd_tree_demo)

add_executable(ch054_kd_tree_tests 054-kd-tree/tests/test_kd_tree.c)
target_link_libraries(ch054_kd_tree_tests PRIVATE ch054_kd_tree)
add_test(NAME ch054_kd_tree_tests COMMAND ch054_kd_tree_tests)

add_executable(ch054_kd_query_benchmark 054-kd-tree/benchmarks/kd_query_benchmark.c)
target_link_libraries(ch054_kd_query_benchmark PRIVATE ch054_kd_tree)


# Chapter 055
add_library(ch055_quadtree
    055-quadtree/src/int_quadtree.c
)
target_include_directories(ch055_quadtree PUBLIC 055-quadtree/src)

add_executable(ch055_quadtree_demo 055-quadtree/examples/quadtree_demo.c)
target_link_libraries(ch055_quadtree_demo PRIVATE ch055_quadtree)
add_test(NAME ch055_quadtree_demo_smoke COMMAND ch055_quadtree_demo)

add_executable(ch055_quadtree_tests 055-quadtree/tests/test_quadtree.c)
target_link_libraries(ch055_quadtree_tests PRIVATE ch055_quadtree)
add_test(NAME ch055_quadtree_tests COMMAND ch055_quadtree_tests)

add_executable(ch055_quadtree_benchmark 055-quadtree/benchmarks/quadtree_benchmark.c)
target_link_libraries(ch055_quadtree_benchmark PRIVATE ch055_quadtree)

# Chapter 056
add_library(ch056_octree
    056-octree/src/int_octree.c
)
target_include_directories(ch056_octree PUBLIC 056-octree/src)

add_executable(ch056_octree_demo 056-octree/examples/octree_demo.c)
target_link_libraries(ch056_octree_demo PRIVATE ch056_octree)
add_test(NAME ch056_octree_demo_smoke COMMAND ch056_octree_demo)

add_executable(ch056_octree_tests 056-octree/tests/test_octree.c)
target_link_libraries(ch056_octree_tests PRIVATE ch056_octree)
add_test(NAME ch056_octree_tests COMMAND ch056_octree_tests)

add_executable(ch056_octree_benchmark 056-octree/benchmarks/octree_benchmark.c)
target_link_libraries(ch056_octree_benchmark PRIVATE ch056_octree)

# Chapter 057
add_library(ch057_r_tree
    057-r-tree/src/int_r_tree.c
)
target_include_directories(ch057_r_tree PUBLIC 057-r-tree/src)

add_executable(ch057_r_tree_demo 057-r-tree/examples/r_tree_demo.c)
target_link_libraries(ch057_r_tree_demo PRIVATE ch057_r_tree)
add_test(NAME ch057_r_tree_demo_smoke COMMAND ch057_r_tree_demo)

add_executable(ch057_r_tree_tests 057-r-tree/tests/test_r_tree.c)
target_link_libraries(ch057_r_tree_tests PRIVATE ch057_r_tree)
add_test(NAME ch057_r_tree_tests COMMAND ch057_r_tree_tests)

add_executable(ch057_r_tree_benchmark 057-r-tree/benchmarks/r_tree_benchmark.c)
target_link_libraries(ch057_r_tree_benchmark PRIVATE ch057_r_tree)


# Chapter 058
add_library(ch058_spatial_hash
    058-spatial-hashing/src/int_spatial_hash.c
)
target_include_directories(ch058_spatial_hash PUBLIC 058-spatial-hashing/src)

add_executable(ch058_spatial_hash_demo 058-spatial-hashing/examples/spatial_hash_demo.c)
target_link_libraries(ch058_spatial_hash_demo PRIVATE ch058_spatial_hash)
add_test(NAME ch058_spatial_hash_demo_smoke COMMAND ch058_spatial_hash_demo)

add_executable(ch058_spatial_hash_tests 058-spatial-hashing/tests/test_spatial_hash.c)
target_link_libraries(ch058_spatial_hash_tests PRIVATE ch058_spatial_hash)
add_test(NAME ch058_spatial_hash_tests COMMAND ch058_spatial_hash_tests)

add_executable(ch058_spatial_hash_benchmark 058-spatial-hashing/benchmarks/spatial_hash_benchmark.c)
target_link_libraries(ch058_spatial_hash_benchmark PRIVATE ch058_spatial_hash)

# Chapter 059
add_library(ch059_suffix_array
    059-suffix-array/src/byte_suffix_array.c
)
target_include_directories(ch059_suffix_array PUBLIC 059-suffix-array/src)

add_executable(ch059_suffix_array_demo 059-suffix-array/examples/suffix_array_demo.c)
target_link_libraries(ch059_suffix_array_demo PRIVATE ch059_suffix_array)
add_test(NAME ch059_suffix_array_demo_smoke COMMAND ch059_suffix_array_demo)

add_executable(ch059_suffix_array_tests 059-suffix-array/tests/test_suffix_array.c)
target_link_libraries(ch059_suffix_array_tests PRIVATE ch059_suffix_array)
add_test(NAME ch059_suffix_array_tests COMMAND ch059_suffix_array_tests)

add_executable(ch059_suffix_array_benchmark 059-suffix-array/benchmarks/suffix_array_benchmark.c)
target_link_libraries(ch059_suffix_array_benchmark PRIVATE ch059_suffix_array)

# Chapter 060
add_library(ch060_suffix_tree
    060-suffix-tree/src/byte_suffix_tree.c
)
target_include_directories(ch060_suffix_tree PUBLIC 060-suffix-tree/src)

add_executable(ch060_suffix_tree_demo 060-suffix-tree/examples/suffix_tree_demo.c)
target_link_libraries(ch060_suffix_tree_demo PRIVATE ch060_suffix_tree)
add_test(NAME ch060_suffix_tree_demo_smoke COMMAND ch060_suffix_tree_demo)

add_executable(ch060_suffix_tree_tests 060-suffix-tree/tests/test_suffix_tree.c)
target_link_libraries(ch060_suffix_tree_tests PRIVATE ch060_suffix_tree)
add_test(NAME ch060_suffix_tree_tests COMMAND ch060_suffix_tree_tests)

add_executable(ch060_suffix_tree_benchmark 060-suffix-tree/benchmarks/suffix_tree_benchmark.c)
target_link_libraries(ch060_suffix_tree_benchmark PRIVATE ch060_suffix_tree)


# Chapter 061
add_library(ch061_suffix_automaton
    061-suffix-automaton/src/byte_suffix_automaton.c
)
target_include_directories(ch061_suffix_automaton PUBLIC 061-suffix-automaton/src)

add_executable(ch061_suffix_automaton_demo 061-suffix-automaton/examples/suffix_automaton_demo.c)
target_link_libraries(ch061_suffix_automaton_demo PRIVATE ch061_suffix_automaton)
add_test(NAME ch061_suffix_automaton_demo_smoke COMMAND ch061_suffix_automaton_demo)

add_executable(ch061_suffix_automaton_tests 061-suffix-automaton/tests/test_suffix_automaton.c)
target_link_libraries(ch061_suffix_automaton_tests PRIVATE ch061_suffix_automaton)
add_test(NAME ch061_suffix_automaton_tests COMMAND ch061_suffix_automaton_tests)

add_executable(ch061_suffix_automaton_benchmark 061-suffix-automaton/benchmarks/suffix_automaton_benchmark.c)
target_link_libraries(ch061_suffix_automaton_benchmark PRIVATE ch061_suffix_automaton)

# Chapter 062
add_library(ch062_aho_corasick
    062-aho-corasick-automaton/src/byte_aho_corasick.c
)
target_include_directories(ch062_aho_corasick PUBLIC 062-aho-corasick-automaton/src)

add_executable(ch062_aho_corasick_demo 062-aho-corasick-automaton/examples/aho_corasick_demo.c)
target_link_libraries(ch062_aho_corasick_demo PRIVATE ch062_aho_corasick)
add_test(NAME ch062_aho_corasick_demo_smoke COMMAND ch062_aho_corasick_demo)

add_executable(ch062_aho_corasick_tests 062-aho-corasick-automaton/tests/test_aho_corasick.c)
target_link_libraries(ch062_aho_corasick_tests PRIVATE ch062_aho_corasick)
add_test(NAME ch062_aho_corasick_tests COMMAND ch062_aho_corasick_tests)

add_executable(ch062_aho_corasick_benchmark 062-aho-corasick-automaton/benchmarks/aho_corasick_benchmark.c)
target_link_libraries(ch062_aho_corasick_benchmark PRIVATE ch062_aho_corasick)

# Chapter 063
add_library(ch063_rolling_hash
    063-rolling-hash-structures/src/byte_rolling_hash.c
)
target_include_directories(ch063_rolling_hash PUBLIC 063-rolling-hash-structures/src)

add_executable(ch063_rolling_hash_demo 063-rolling-hash-structures/examples/rolling_hash_demo.c)
target_link_libraries(ch063_rolling_hash_demo PRIVATE ch063_rolling_hash)
add_test(NAME ch063_rolling_hash_demo_smoke COMMAND ch063_rolling_hash_demo)

add_executable(ch063_rolling_hash_tests 063-rolling-hash-structures/tests/test_rolling_hash.c)
target_link_libraries(ch063_rolling_hash_tests PRIVATE ch063_rolling_hash)
add_test(NAME ch063_rolling_hash_tests COMMAND ch063_rolling_hash_tests)

add_executable(ch063_rolling_hash_benchmark 063-rolling-hash-structures/benchmarks/rolling_hash_benchmark.c)
target_link_libraries(ch063_rolling_hash_benchmark PRIVATE ch063_rolling_hash)


# Chapter 064
add_library(ch064_bitset
    064-bitset/src/int_bitset.c
)
target_include_directories(ch064_bitset PUBLIC 064-bitset/src)

add_executable(ch064_bitset_demo 064-bitset/examples/bitset_demo.c)
target_link_libraries(ch064_bitset_demo PRIVATE ch064_bitset)
add_test(NAME ch064_bitset_demo_smoke COMMAND ch064_bitset_demo)

add_executable(ch064_bitset_tests 064-bitset/tests/test_bitset.c)
target_link_libraries(ch064_bitset_tests PRIVATE ch064_bitset)
add_test(NAME ch064_bitset_tests COMMAND ch064_bitset_tests)

add_executable(ch064_bitset_benchmark 064-bitset/benchmarks/bitset_benchmark.c)
target_link_libraries(ch064_bitset_benchmark PRIVATE ch064_bitset)

# Chapter 065
add_library(ch065_bitmap
    065-bitmap/src/int_bitmap.c
)
target_include_directories(ch065_bitmap PUBLIC 065-bitmap/src)

add_executable(ch065_bitmap_demo 065-bitmap/examples/bitmap_demo.c)
target_link_libraries(ch065_bitmap_demo PRIVATE ch065_bitmap)
add_test(NAME ch065_bitmap_demo_smoke COMMAND ch065_bitmap_demo)

add_executable(ch065_bitmap_tests 065-bitmap/tests/test_bitmap.c)
target_link_libraries(ch065_bitmap_tests PRIVATE ch065_bitmap)
add_test(NAME ch065_bitmap_tests COMMAND ch065_bitmap_tests)

add_executable(ch065_bitmap_benchmark 065-bitmap/benchmarks/bitmap_benchmark.c)
target_link_libraries(ch065_bitmap_benchmark PRIVATE ch065_bitmap)

# Chapter 066
add_library(ch066_bit_vector
    066-bit-vector/src/int_bit_vector.c
)
target_include_directories(ch066_bit_vector PUBLIC 066-bit-vector/src)

add_executable(ch066_bit_vector_demo 066-bit-vector/examples/bit_vector_demo.c)
target_link_libraries(ch066_bit_vector_demo PRIVATE ch066_bit_vector)
add_test(NAME ch066_bit_vector_demo_smoke COMMAND ch066_bit_vector_demo)

add_executable(ch066_bit_vector_tests 066-bit-vector/tests/test_bit_vector.c)
target_link_libraries(ch066_bit_vector_tests PRIVATE ch066_bit_vector)
add_test(NAME ch066_bit_vector_tests COMMAND ch066_bit_vector_tests)

add_executable(ch066_bit_vector_benchmark 066-bit-vector/benchmarks/bit_vector_benchmark.c)
target_link_libraries(ch066_bit_vector_benchmark PRIVATE ch066_bit_vector)


# Chapter 067
add_library(ch067_xor_trie
    067-bit-trie-xor-trie/src/int_xor_trie.c
)
target_include_directories(ch067_xor_trie PUBLIC 067-bit-trie-xor-trie/src)

add_executable(ch067_xor_trie_demo 067-bit-trie-xor-trie/examples/xor_trie_demo.c)
target_link_libraries(ch067_xor_trie_demo PRIVATE ch067_xor_trie)
add_test(NAME ch067_xor_trie_demo_smoke COMMAND ch067_xor_trie_demo)

add_executable(ch067_xor_trie_tests 067-bit-trie-xor-trie/tests/test_xor_trie.c)
target_link_libraries(ch067_xor_trie_tests PRIVATE ch067_xor_trie)
add_test(NAME ch067_xor_trie_tests COMMAND ch067_xor_trie_tests)

add_executable(ch067_xor_trie_benchmark 067-bit-trie-xor-trie/benchmarks/xor_trie_benchmark.c)
target_link_libraries(ch067_xor_trie_benchmark PRIVATE ch067_xor_trie)

# Chapter 068
add_library(ch068_bloom_filter
    068-bloom-filter/src/byte_bloom_filter.c
)
target_include_directories(ch068_bloom_filter PUBLIC 068-bloom-filter/src)

add_executable(ch068_bloom_filter_demo 068-bloom-filter/examples/bloom_filter_demo.c)
target_link_libraries(ch068_bloom_filter_demo PRIVATE ch068_bloom_filter)
add_test(NAME ch068_bloom_filter_demo_smoke COMMAND ch068_bloom_filter_demo)

add_executable(ch068_bloom_filter_tests 068-bloom-filter/tests/test_bloom_filter.c)
target_link_libraries(ch068_bloom_filter_tests PRIVATE ch068_bloom_filter)
add_test(NAME ch068_bloom_filter_tests COMMAND ch068_bloom_filter_tests)

add_executable(ch068_bloom_filter_benchmark 068-bloom-filter/benchmarks/bloom_filter_benchmark.c)
target_link_libraries(ch068_bloom_filter_benchmark PRIVATE ch068_bloom_filter)

# Chapter 069
add_library(ch069_counting_bloom
    069-counting-bloom-filter/src/byte_counting_bloom.c
)
target_include_directories(ch069_counting_bloom PUBLIC 069-counting-bloom-filter/src)

add_executable(ch069_counting_bloom_demo 069-counting-bloom-filter/examples/counting_bloom_demo.c)
target_link_libraries(ch069_counting_bloom_demo PRIVATE ch069_counting_bloom)
add_test(NAME ch069_counting_bloom_demo_smoke COMMAND ch069_counting_bloom_demo)

add_executable(ch069_counting_bloom_tests 069-counting-bloom-filter/tests/test_counting_bloom.c)
target_link_libraries(ch069_counting_bloom_tests PRIVATE ch069_counting_bloom)
add_test(NAME ch069_counting_bloom_tests COMMAND ch069_counting_bloom_tests)

add_executable(ch069_counting_bloom_benchmark 069-counting-bloom-filter/benchmarks/counting_bloom_benchmark.c)
target_link_libraries(ch069_counting_bloom_benchmark PRIVATE ch069_counting_bloom)


# Chapter 070
add_library(ch070_cuckoo_filter
    070-cuckoo-filter/src/byte_cuckoo_filter.c
)
target_include_directories(ch070_cuckoo_filter PUBLIC 070-cuckoo-filter/src)

add_executable(ch070_cuckoo_filter_demo 070-cuckoo-filter/examples/cuckoo_filter_demo.c)
target_link_libraries(ch070_cuckoo_filter_demo PRIVATE ch070_cuckoo_filter)
add_test(NAME ch070_cuckoo_filter_demo_smoke COMMAND ch070_cuckoo_filter_demo)

add_executable(ch070_cuckoo_filter_tests 070-cuckoo-filter/tests/test_cuckoo_filter.c)
target_link_libraries(ch070_cuckoo_filter_tests PRIVATE ch070_cuckoo_filter)
add_test(NAME ch070_cuckoo_filter_tests COMMAND ch070_cuckoo_filter_tests)

add_executable(ch070_cuckoo_filter_benchmark 070-cuckoo-filter/benchmarks/cuckoo_filter_benchmark.c)
target_link_libraries(ch070_cuckoo_filter_benchmark PRIVATE ch070_cuckoo_filter)

# Chapter 071
add_library(ch071_cuckoo_hashing
    071-cuckoo-hashing/src/u64_cuckoo_set.c
)
target_include_directories(ch071_cuckoo_hashing PUBLIC 071-cuckoo-hashing/src)

add_executable(ch071_cuckoo_hashing_demo 071-cuckoo-hashing/examples/cuckoo_hashing_demo.c)
target_link_libraries(ch071_cuckoo_hashing_demo PRIVATE ch071_cuckoo_hashing)
add_test(NAME ch071_cuckoo_hashing_demo_smoke COMMAND ch071_cuckoo_hashing_demo)

add_executable(ch071_cuckoo_hashing_tests 071-cuckoo-hashing/tests/test_cuckoo_hashing.c)
target_link_libraries(ch071_cuckoo_hashing_tests PRIVATE ch071_cuckoo_hashing)
add_test(NAME ch071_cuckoo_hashing_tests COMMAND ch071_cuckoo_hashing_tests)

add_executable(ch071_cuckoo_hashing_benchmark 071-cuckoo-hashing/benchmarks/cuckoo_hashing_benchmark.c)
target_link_libraries(ch071_cuckoo_hashing_benchmark PRIVATE ch071_cuckoo_hashing)

# Chapter 072
add_library(ch072_perfect_hashing
    072-perfect-hashing/src/u64_perfect_set.c
)
target_include_directories(ch072_perfect_hashing PUBLIC 072-perfect-hashing/src)

add_executable(ch072_perfect_hashing_demo 072-perfect-hashing/examples/perfect_hashing_demo.c)
target_link_libraries(ch072_perfect_hashing_demo PRIVATE ch072_perfect_hashing)
add_test(NAME ch072_perfect_hashing_demo_smoke COMMAND ch072_perfect_hashing_demo)

add_executable(ch072_perfect_hashing_tests 072-perfect-hashing/tests/test_perfect_hashing.c)
target_link_libraries(ch072_perfect_hashing_tests PRIVATE ch072_perfect_hashing)
add_test(NAME ch072_perfect_hashing_tests COMMAND ch072_perfect_hashing_tests)

add_executable(ch072_perfect_hashing_benchmark 072-perfect-hashing/benchmarks/perfect_hashing_benchmark.c)
target_link_libraries(ch072_perfect_hashing_benchmark PRIVATE ch072_perfect_hashing)


# Chapter 073
add_library(ch073_consistent_hashing
    073-consistent-hashing/src/consistent_hash_ring.c
)
target_include_directories(ch073_consistent_hashing PUBLIC 073-consistent-hashing/src)

add_executable(ch073_consistent_hashing_demo 073-consistent-hashing/examples/consistent_hashing_demo.c)
target_link_libraries(ch073_consistent_hashing_demo PRIVATE ch073_consistent_hashing)
add_test(NAME ch073_consistent_hashing_demo_smoke COMMAND ch073_consistent_hashing_demo)

add_executable(ch073_consistent_hashing_tests 073-consistent-hashing/tests/test_consistent_hashing.c)
target_link_libraries(ch073_consistent_hashing_tests PRIVATE ch073_consistent_hashing)
add_test(NAME ch073_consistent_hashing_tests COMMAND ch073_consistent_hashing_tests)

add_executable(ch073_consistent_hashing_benchmark 073-consistent-hashing/benchmarks/consistent_hashing_benchmark.c)
target_link_libraries(ch073_consistent_hashing_benchmark PRIVATE ch073_consistent_hashing)

# Chapter 074
add_library(ch074_kmv
    074-probabilistic-data-structures/src/u64_kmv_sketch.c
)
target_include_directories(ch074_kmv PUBLIC 074-probabilistic-data-structures/src)

add_executable(ch074_kmv_demo 074-probabilistic-data-structures/examples/kmv_demo.c)
target_link_libraries(ch074_kmv_demo PRIVATE ch074_kmv)
add_test(NAME ch074_kmv_demo_smoke COMMAND ch074_kmv_demo)

add_executable(ch074_probabilistic_tests 074-probabilistic-data-structures/tests/test_probabilistic_structures.c)
target_link_libraries(ch074_probabilistic_tests PRIVATE ch074_kmv m)
add_test(NAME ch074_probabilistic_tests COMMAND ch074_probabilistic_tests)

add_executable(ch074_kmv_benchmark 074-probabilistic-data-structures/benchmarks/kmv_benchmark.c)
target_link_libraries(ch074_kmv_benchmark PRIVATE ch074_kmv)

# Chapter 075
add_library(ch075_hyperloglog
    075-hyperloglog/src/u64_hyperloglog.c
)
target_include_directories(ch075_hyperloglog PUBLIC 075-hyperloglog/src)
target_link_libraries(ch075_hyperloglog PUBLIC m)

add_executable(ch075_hyperloglog_demo 075-hyperloglog/examples/hyperloglog_demo.c)
target_link_libraries(ch075_hyperloglog_demo PRIVATE ch075_hyperloglog)
add_test(NAME ch075_hyperloglog_demo_smoke COMMAND ch075_hyperloglog_demo)

add_executable(ch075_hyperloglog_tests 075-hyperloglog/tests/test_hyperloglog.c)
target_link_libraries(ch075_hyperloglog_tests PRIVATE ch075_hyperloglog)
add_test(NAME ch075_hyperloglog_tests COMMAND ch075_hyperloglog_tests)

add_executable(ch075_hyperloglog_benchmark 075-hyperloglog/benchmarks/hyperloglog_benchmark.c)
target_link_libraries(ch075_hyperloglog_benchmark PRIVATE ch075_hyperloglog)


# Chapter 076
add_library(ch076_count_min
    076-count-min-sketch/src/u64_count_min_sketch.c
)
target_include_directories(ch076_count_min PUBLIC 076-count-min-sketch/src)

add_executable(ch076_count_min_demo 076-count-min-sketch/examples/count_min_demo.c)
target_link_libraries(ch076_count_min_demo PRIVATE ch076_count_min)
add_test(NAME ch076_count_min_demo_smoke COMMAND ch076_count_min_demo)

add_executable(ch076_count_min_sketch_tests 076-count-min-sketch/tests/test_count_min_sketch.c)
target_link_libraries(ch076_count_min_sketch_tests PRIVATE ch076_count_min)
add_test(NAME ch076_count_min_sketch_tests COMMAND ch076_count_min_sketch_tests)

add_executable(ch076_count_min_benchmark 076-count-min-sketch/benchmarks/count_min_benchmark.c)
target_link_libraries(ch076_count_min_benchmark PRIVATE ch076_count_min)

# Chapter 077
add_library(ch077_count_sketch
    077-count-sketch/src/u64_count_sketch.c
)
target_include_directories(ch077_count_sketch PUBLIC 077-count-sketch/src)

add_executable(ch077_count_sketch_demo 077-count-sketch/examples/count_sketch_demo.c)
target_link_libraries(ch077_count_sketch_demo PRIVATE ch077_count_sketch)
add_test(NAME ch077_count_sketch_demo_smoke COMMAND ch077_count_sketch_demo)

add_executable(ch077_count_sketch_tests 077-count-sketch/tests/test_count_sketch.c)
target_link_libraries(ch077_count_sketch_tests PRIVATE ch077_count_sketch)
add_test(NAME ch077_count_sketch_tests COMMAND ch077_count_sketch_tests)

add_executable(ch077_count_sketch_benchmark 077-count-sketch/benchmarks/count_sketch_benchmark.c)
target_link_libraries(ch077_count_sketch_benchmark PRIVATE ch077_count_sketch)

# Chapter 078
add_library(ch078_reservoir
    078-reservoir-sampling-structures/src/u64_reservoir.c
)
target_include_directories(ch078_reservoir PUBLIC 078-reservoir-sampling-structures/src)

add_executable(ch078_reservoir_demo 078-reservoir-sampling-structures/examples/reservoir_demo.c)
target_link_libraries(ch078_reservoir_demo PRIVATE ch078_reservoir)
add_test(NAME ch078_reservoir_demo_smoke COMMAND ch078_reservoir_demo)

add_executable(ch078_reservoir_sampling_tests 078-reservoir-sampling-structures/tests/test_reservoir_sampling.c)
target_link_libraries(ch078_reservoir_sampling_tests PRIVATE ch078_reservoir)
add_test(NAME ch078_reservoir_sampling_tests COMMAND ch078_reservoir_sampling_tests)

add_executable(ch078_reservoir_benchmark 078-reservoir-sampling-structures/benchmarks/reservoir_benchmark.c)
target_link_libraries(ch078_reservoir_benchmark PRIVATE ch078_reservoir)


# Chapter 079
add_library(ch079_linked_hash_map
    079-linked-hash-map/src/u64_linked_hash_map.c
)
target_include_directories(ch079_linked_hash_map PUBLIC 079-linked-hash-map/src)

add_executable(ch079_linked_hash_map_demo 079-linked-hash-map/examples/linked_hash_map_demo.c)
target_link_libraries(ch079_linked_hash_map_demo PRIVATE ch079_linked_hash_map)
add_test(NAME ch079_linked_hash_map_demo_smoke COMMAND ch079_linked_hash_map_demo)

add_executable(ch079_linked_hash_map_tests 079-linked-hash-map/tests/test_linked_hash_map.c)
target_link_libraries(ch079_linked_hash_map_tests PRIVATE ch079_linked_hash_map)
add_test(NAME ch079_linked_hash_map_tests COMMAND ch079_linked_hash_map_tests)

add_executable(ch079_linked_hash_map_benchmark 079-linked-hash-map/benchmarks/linked_hash_map_benchmark.c)
target_link_libraries(ch079_linked_hash_map_benchmark PRIVATE ch079_linked_hash_map)

# Chapter 080
add_library(ch080_ordered
    080-ordered-map-ordered-set/src/u64_ordered.c
)
target_include_directories(ch080_ordered PUBLIC 080-ordered-map-ordered-set/src)

add_executable(ch080_ordered_demo 080-ordered-map-ordered-set/examples/ordered_demo.c)
target_link_libraries(ch080_ordered_demo PRIVATE ch080_ordered)
add_test(NAME ch080_ordered_demo_smoke COMMAND ch080_ordered_demo)

add_executable(ch080_ordered_tests 080-ordered-map-ordered-set/tests/test_ordered.c)
target_link_libraries(ch080_ordered_tests PRIVATE ch080_ordered)
add_test(NAME ch080_ordered_tests COMMAND ch080_ordered_tests)

add_executable(ch080_ordered_benchmark 080-ordered-map-ordered-set/benchmarks/ordered_benchmark.c)
target_link_libraries(ch080_ordered_benchmark PRIVATE ch080_ordered)

# Chapter 081
add_library(ch081_multi
    081-multiset-multimap/src/u64_multi.c
)
target_include_directories(ch081_multi PUBLIC 081-multiset-multimap/src)

add_executable(ch081_multi_demo 081-multiset-multimap/examples/multi_demo.c)
target_link_libraries(ch081_multi_demo PRIVATE ch081_multi)
add_test(NAME ch081_multi_demo_smoke COMMAND ch081_multi_demo)

add_executable(ch081_multi_tests 081-multiset-multimap/tests/test_multi.c)
target_link_libraries(ch081_multi_tests PRIVATE ch081_multi)
add_test(NAME ch081_multi_tests COMMAND ch081_multi_tests)

add_executable(ch081_multi_benchmark 081-multiset-multimap/benchmarks/multi_benchmark.c)
target_link_libraries(ch081_multi_benchmark PRIVATE ch081_multi)


# Chapter 082
add_library(ch082_ring_buffer
    082-circular-buffer-ring-buffer/src/u64_ring_buffer.c
)
target_include_directories(ch082_ring_buffer PUBLIC 082-circular-buffer-ring-buffer/src)

add_executable(ch082_ring_buffer_demo 082-circular-buffer-ring-buffer/examples/ring_buffer_demo.c)
target_link_libraries(ch082_ring_buffer_demo PRIVATE ch082_ring_buffer)
add_test(NAME ch082_ring_buffer_demo_smoke COMMAND ch082_ring_buffer_demo)

add_executable(ch082_ring_buffer_tests 082-circular-buffer-ring-buffer/tests/test_ring_buffer.c)
target_link_libraries(ch082_ring_buffer_tests PRIVATE ch082_ring_buffer)
add_test(NAME ch082_ring_buffer_tests COMMAND ch082_ring_buffer_tests)

add_executable(ch082_ring_buffer_benchmark 082-circular-buffer-ring-buffer/benchmarks/ring_buffer_benchmark.c)
target_link_libraries(ch082_ring_buffer_benchmark PRIVATE ch082_ring_buffer)

# Chapter 083
add_library(ch083_gap_buffer
    083-gap-buffer/src/gap_buffer.c
)
target_include_directories(ch083_gap_buffer PUBLIC 083-gap-buffer/src)

add_executable(ch083_gap_buffer_demo 083-gap-buffer/examples/gap_buffer_demo.c)
target_link_libraries(ch083_gap_buffer_demo PRIVATE ch083_gap_buffer)
add_test(NAME ch083_gap_buffer_demo_smoke COMMAND ch083_gap_buffer_demo)

add_executable(ch083_gap_buffer_tests 083-gap-buffer/tests/test_gap_buffer.c)
target_link_libraries(ch083_gap_buffer_tests PRIVATE ch083_gap_buffer)
add_test(NAME ch083_gap_buffer_tests COMMAND ch083_gap_buffer_tests)

add_executable(ch083_gap_buffer_benchmark 083-gap-buffer/benchmarks/gap_buffer_benchmark.c)
target_link_libraries(ch083_gap_buffer_benchmark PRIVATE ch083_gap_buffer)

# Chapter 084
add_library(ch084_rope
    084-rope/src/byte_rope.c
)
target_include_directories(ch084_rope PUBLIC 084-rope/src)

add_executable(ch084_rope_demo 084-rope/examples/rope_demo.c)
target_link_libraries(ch084_rope_demo PRIVATE ch084_rope)
add_test(NAME ch084_rope_demo_smoke COMMAND ch084_rope_demo)

add_executable(ch084_rope_tests 084-rope/tests/test_rope.c)
target_link_libraries(ch084_rope_tests PRIVATE ch084_rope)
add_test(NAME ch084_rope_tests COMMAND ch084_rope_tests)

add_executable(ch084_rope_benchmark 084-rope/benchmarks/rope_benchmark.c)
target_link_libraries(ch084_rope_benchmark PRIVATE ch084_rope)


# Chapter 085
add_library(ch085_piece_table
    085-piece-table/src/piece_table.c
)
target_include_directories(ch085_piece_table PUBLIC 085-piece-table/src)

add_executable(ch085_piece_table_demo 085-piece-table/examples/piece_table_demo.c)
target_link_libraries(ch085_piece_table_demo PRIVATE ch085_piece_table)
add_test(NAME ch085_piece_table_demo_smoke COMMAND ch085_piece_table_demo)

add_executable(ch085_piece_table_tests 085-piece-table/tests/test_piece_table.c)
target_link_libraries(ch085_piece_table_tests PRIVATE ch085_piece_table)
add_test(NAME ch085_piece_table_tests COMMAND ch085_piece_table_tests)

add_executable(ch085_piece_table_benchmark 085-piece-table/benchmarks/piece_table_benchmark.c)
target_link_libraries(ch085_piece_table_benchmark PRIVATE ch085_piece_table)

# Chapter 086
add_library(ch086_inverted_index
    086-inverted-index/src/inverted_index.c
)
target_include_directories(ch086_inverted_index PUBLIC 086-inverted-index/src)

add_executable(ch086_inverted_index_demo 086-inverted-index/examples/inverted_index_demo.c)
target_link_libraries(ch086_inverted_index_demo PRIVATE ch086_inverted_index)
add_test(NAME ch086_inverted_index_demo_smoke COMMAND ch086_inverted_index_demo)

add_executable(ch086_inverted_index_tests 086-inverted-index/tests/test_inverted_index.c)
target_link_libraries(ch086_inverted_index_tests PRIVATE ch086_inverted_index)
add_test(NAME ch086_inverted_index_tests COMMAND ch086_inverted_index_tests)

add_executable(ch086_inverted_index_benchmark 086-inverted-index/benchmarks/inverted_index_benchmark.c)
target_link_libraries(ch086_inverted_index_benchmark PRIVATE ch086_inverted_index)

# Chapter 087
add_library(ch087_posting_list
    087-posting-list/src/posting_list.c
)
target_include_directories(ch087_posting_list PUBLIC 087-posting-list/src)

add_executable(ch087_posting_list_demo 087-posting-list/examples/posting_list_demo.c)
target_link_libraries(ch087_posting_list_demo PRIVATE ch087_posting_list)
add_test(NAME ch087_posting_list_demo_smoke COMMAND ch087_posting_list_demo)

add_executable(ch087_posting_list_tests 087-posting-list/tests/test_posting_list.c)
target_link_libraries(ch087_posting_list_tests PRIVATE ch087_posting_list)
add_test(NAME ch087_posting_list_tests COMMAND ch087_posting_list_tests)

add_executable(ch087_posting_list_benchmark 087-posting-list/benchmarks/posting_list_benchmark.c)
target_link_libraries(ch087_posting_list_benchmark PRIVATE ch087_posting_list)


# Chapter 088
add_library(ch088_sparse_matrix
    088-sparse-matrix-representations/src/sparse_matrix.c
)
target_include_directories(ch088_sparse_matrix PUBLIC 088-sparse-matrix-representations/src)

add_executable(ch088_sparse_matrix_demo 088-sparse-matrix-representations/examples/sparse_matrix_demo.c)
target_link_libraries(ch088_sparse_matrix_demo PRIVATE ch088_sparse_matrix)
add_test(NAME ch088_sparse_matrix_demo_smoke COMMAND ch088_sparse_matrix_demo)

add_executable(ch088_sparse_matrix_tests 088-sparse-matrix-representations/tests/test_sparse_matrix.c)
target_link_libraries(ch088_sparse_matrix_tests PRIVATE ch088_sparse_matrix m)
add_test(NAME ch088_sparse_matrix_tests COMMAND ch088_sparse_matrix_tests)

add_executable(ch088_sparse_matrix_benchmark 088-sparse-matrix-representations/benchmarks/sparse_matrix_benchmark.c)
target_link_libraries(ch088_sparse_matrix_benchmark PRIVATE ch088_sparse_matrix)

# Chapter 089
add_library(ch089_tensor_layout
    089-matrix-tensor-storage-layout/src/tensor_layout.c
)
target_include_directories(ch089_tensor_layout PUBLIC 089-matrix-tensor-storage-layout/src)

add_executable(ch089_tensor_layout_demo 089-matrix-tensor-storage-layout/examples/tensor_layout_demo.c)
target_link_libraries(ch089_tensor_layout_demo PRIVATE ch089_tensor_layout)
add_test(NAME ch089_tensor_layout_demo_smoke COMMAND ch089_tensor_layout_demo)

add_executable(ch089_tensor_layout_tests 089-matrix-tensor-storage-layout/tests/test_tensor_layout.c)
target_link_libraries(ch089_tensor_layout_tests PRIVATE ch089_tensor_layout)
add_test(NAME ch089_tensor_layout_tests COMMAND ch089_tensor_layout_tests)

add_executable(ch089_tensor_layout_benchmark 089-matrix-tensor-storage-layout/benchmarks/tensor_layout_benchmark.c)
target_link_libraries(ch089_tensor_layout_benchmark PRIVATE ch089_tensor_layout)

# Chapter 090
add_library(ch090_compressed_seq
    090-compressed-data-structures/src/u64_compressed_seq.c
)
target_include_directories(ch090_compressed_seq PUBLIC 090-compressed-data-structures/src)

add_executable(ch090_compressed_seq_demo 090-compressed-data-structures/examples/compressed_seq_demo.c)
target_link_libraries(ch090_compressed_seq_demo PRIVATE ch090_compressed_seq)
add_test(NAME ch090_compressed_seq_demo_smoke COMMAND ch090_compressed_seq_demo)

add_executable(ch090_compressed_seq_tests 090-compressed-data-structures/tests/test_compressed_seq.c)
target_link_libraries(ch090_compressed_seq_tests PRIVATE ch090_compressed_seq)
add_test(NAME ch090_compressed_seq_tests COMMAND ch090_compressed_seq_tests)

add_executable(ch090_compressed_seq_benchmark 090-compressed-data-structures/benchmarks/compressed_seq_benchmark.c)
target_link_libraries(ch090_compressed_seq_benchmark PRIVATE ch090_compressed_seq)


# Chapter 091
add_library(ch091_succinct
    091-succinct-data-structures/src/succinct_bit_vector.c
)
target_include_directories(ch091_succinct PUBLIC 091-succinct-data-structures/src)

add_executable(ch091_succinct_demo 091-succinct-data-structures/examples/succinct_bit_vector_demo.c)
target_link_libraries(ch091_succinct_demo PRIVATE ch091_succinct)
add_test(NAME ch091_succinct_demo_smoke COMMAND ch091_succinct_demo)

add_executable(ch091_succinct_tests 091-succinct-data-structures/tests/test_succinct_bit_vector.c)
target_link_libraries(ch091_succinct_tests PRIVATE ch091_succinct)
add_test(NAME ch091_succinct_tests COMMAND ch091_succinct_tests)

add_executable(ch091_succinct_benchmark 091-succinct-data-structures/benchmarks/succinct_bit_vector_benchmark.c)
target_link_libraries(ch091_succinct_benchmark PRIVATE ch091_succinct)

# Chapter 092
add_library(ch092_persistent
    092-persistent-data-structures/src/persistent_int_set.c
)
target_include_directories(ch092_persistent PUBLIC 092-persistent-data-structures/src)

add_executable(ch092_persistent_demo 092-persistent-data-structures/examples/persistent_int_set_demo.c)
target_link_libraries(ch092_persistent_demo PRIVATE ch092_persistent)
add_test(NAME ch092_persistent_demo_smoke COMMAND ch092_persistent_demo)

add_executable(ch092_persistent_tests 092-persistent-data-structures/tests/test_persistent_int_set.c)
target_link_libraries(ch092_persistent_tests PRIVATE ch092_persistent)
add_test(NAME ch092_persistent_tests COMMAND ch092_persistent_tests)

add_executable(ch092_persistent_benchmark 092-persistent-data-structures/benchmarks/persistent_int_set_benchmark.c)
target_link_libraries(ch092_persistent_benchmark PRIVATE ch092_persistent)

# Chapter 093
add_library(ch093_immutable
    093-immutable-data-structures/src/frozen_int_set.c
)
target_include_directories(ch093_immutable PUBLIC 093-immutable-data-structures/src)

add_executable(ch093_immutable_demo 093-immutable-data-structures/examples/frozen_int_set_demo.c)
target_link_libraries(ch093_immutable_demo PRIVATE ch093_immutable)
add_test(NAME ch093_immutable_demo_smoke COMMAND ch093_immutable_demo)

add_executable(ch093_immutable_tests 093-immutable-data-structures/tests/test_frozen_int_set.c)
target_link_libraries(ch093_immutable_tests PRIVATE ch093_immutable)
add_test(NAME ch093_immutable_tests COMMAND ch093_immutable_tests)

add_executable(ch093_immutable_benchmark 093-immutable-data-structures/benchmarks/frozen_int_set_benchmark.c)
target_link_libraries(ch093_immutable_benchmark PRIVATE ch093_immutable)


# Chapter 094
add_library(ch094_functional
    094-functional-data-structures/src/functional_queue.c
)
target_include_directories(ch094_functional PUBLIC 094-functional-data-structures/src)

add_executable(ch094_functional_demo 094-functional-data-structures/examples/functional_queue_demo.c)
target_link_libraries(ch094_functional_demo PRIVATE ch094_functional)
add_test(NAME ch094_functional_demo_smoke COMMAND ch094_functional_demo)

add_executable(ch094_functional_tests 094-functional-data-structures/tests/test_functional_queue.c)
target_link_libraries(ch094_functional_tests PRIVATE ch094_functional)
add_test(NAME ch094_functional_tests COMMAND ch094_functional_tests)

add_executable(ch094_functional_benchmark 094-functional-data-structures/benchmarks/functional_queue_benchmark.c)
target_link_libraries(ch094_functional_benchmark PRIVATE ch094_functional)

# Chapter 095
add_library(ch095_cow
    095-copy-on-write-structures/src/cow_int_vector.c
)
target_include_directories(ch095_cow PUBLIC 095-copy-on-write-structures/src)

add_executable(ch095_cow_demo 095-copy-on-write-structures/examples/cow_int_vector_demo.c)
target_link_libraries(ch095_cow_demo PRIVATE ch095_cow)
add_test(NAME ch095_cow_demo_smoke COMMAND ch095_cow_demo)

add_executable(ch095_cow_tests 095-copy-on-write-structures/tests/test_cow_int_vector.c)
target_link_libraries(ch095_cow_tests PRIVATE ch095_cow)
add_test(NAME ch095_cow_tests COMMAND ch095_cow_tests)

add_executable(ch095_cow_benchmark 095-copy-on-write-structures/benchmarks/cow_int_vector_benchmark.c)
target_link_libraries(ch095_cow_benchmark PRIVATE ch095_cow)

# Chapter 096
find_package(Threads REQUIRED)

add_library(ch096_concurrent
    096-concurrent-data-structures/src/concurrent_int_set.c
)
target_include_directories(ch096_concurrent PUBLIC 096-concurrent-data-structures/src)
target_link_libraries(ch096_concurrent PUBLIC Threads::Threads)

add_executable(ch096_concurrent_demo 096-concurrent-data-structures/examples/concurrent_int_set_demo.c)
target_link_libraries(ch096_concurrent_demo PRIVATE ch096_concurrent)
add_test(NAME ch096_concurrent_demo_smoke COMMAND ch096_concurrent_demo)

add_executable(ch096_concurrent_tests 096-concurrent-data-structures/tests/test_concurrent_int_set.c)
target_link_libraries(ch096_concurrent_tests PRIVATE ch096_concurrent)
add_test(NAME ch096_concurrent_tests COMMAND ch096_concurrent_tests)

add_executable(ch096_atomic_publication 096-concurrent-data-structures/examples/atomic_publication_demo.c)
target_link_libraries(ch096_atomic_publication PRIVATE Threads::Threads)
add_test(NAME ch096_atomic_publication COMMAND ch096_atomic_publication)

add_executable(ch096_concurrent_benchmark 096-concurrent-data-structures/benchmarks/concurrent_int_set_benchmark.c)
target_link_libraries(ch096_concurrent_benchmark PRIVATE ch096_concurrent)


# Chapter 097
add_library(ch097_thread_safe
    097-thread-safe-queue-map/src/thread_safe_structures.c
)
target_include_directories(ch097_thread_safe PUBLIC 097-thread-safe-queue-map/src)
target_link_libraries(ch097_thread_safe PUBLIC Threads::Threads)

add_executable(ch097_thread_safe_demo 097-thread-safe-queue-map/examples/thread_safe_demo.c)
target_link_libraries(ch097_thread_safe_demo PRIVATE ch097_thread_safe)
add_test(NAME ch097_thread_safe_demo_smoke COMMAND ch097_thread_safe_demo)

add_executable(ch097_thread_safe_tests 097-thread-safe-queue-map/tests/test_thread_safe_structures.c)
target_link_libraries(ch097_thread_safe_tests PRIVATE ch097_thread_safe)
add_test(NAME ch097_thread_safe_tests COMMAND ch097_thread_safe_tests)

add_executable(ch097_thread_safe_benchmark 097-thread-safe-queue-map/benchmarks/thread_safe_benchmark.c)
target_link_libraries(ch097_thread_safe_benchmark PRIVATE ch097_thread_safe)

# Chapter 098
add_library(ch098_lock_free
    098-lock-free-data-structures/src/lock_free_stack.c
)
target_include_directories(ch098_lock_free PUBLIC 098-lock-free-data-structures/src)
target_link_libraries(ch098_lock_free PUBLIC Threads::Threads)

add_executable(ch098_lock_free_demo 098-lock-free-data-structures/examples/lock_free_stack_demo.c)
target_link_libraries(ch098_lock_free_demo PRIVATE ch098_lock_free)
add_test(NAME ch098_lock_free_demo_smoke COMMAND ch098_lock_free_demo)

add_executable(ch098_lock_free_tests 098-lock-free-data-structures/tests/test_lock_free_stack.c)
target_link_libraries(ch098_lock_free_tests PRIVATE ch098_lock_free)
add_test(NAME ch098_lock_free_tests COMMAND ch098_lock_free_tests)

add_executable(ch098_lock_free_benchmark 098-lock-free-data-structures/benchmarks/lock_free_stack_benchmark.c)
target_link_libraries(ch098_lock_free_benchmark PRIVATE ch098_lock_free)

# Chapter 099
add_library(ch099_wait_free
    099-wait-free-data-structures/src/wait_free_spsc_ring.c
)
target_include_directories(ch099_wait_free PUBLIC 099-wait-free-data-structures/src)
target_link_libraries(ch099_wait_free PUBLIC Threads::Threads)

add_executable(ch099_wait_free_demo 099-wait-free-data-structures/examples/wait_free_spsc_demo.c)
target_link_libraries(ch099_wait_free_demo PRIVATE ch099_wait_free)
add_test(NAME ch099_wait_free_demo_smoke COMMAND ch099_wait_free_demo)

add_executable(ch099_wait_free_tests 099-wait-free-data-structures/tests/test_wait_free_spsc_ring.c)
target_link_libraries(ch099_wait_free_tests PRIVATE ch099_wait_free)
add_test(NAME ch099_wait_free_tests COMMAND ch099_wait_free_tests)

add_executable(ch099_wait_free_benchmark 099-wait-free-data-structures/benchmarks/wait_free_spsc_benchmark.c)
target_link_libraries(ch099_wait_free_benchmark PRIVATE ch099_wait_free)


# Chapter 100
add_library(ch100_atomic
    100-atomic-data-structures-cas/src/atomic_structures.c
)
target_include_directories(ch100_atomic PUBLIC 100-atomic-data-structures-cas/src)

add_executable(ch100_atomic_demo 100-atomic-data-structures-cas/examples/atomic_structures_demo.c)
target_link_libraries(ch100_atomic_demo PRIVATE ch100_atomic)
add_test(NAME ch100_atomic_demo_smoke COMMAND ch100_atomic_demo)

add_executable(ch100_atomic_tests 100-atomic-data-structures-cas/tests/test_atomic_structures.c)
target_link_libraries(ch100_atomic_tests PRIVATE ch100_atomic Threads::Threads)
add_test(NAME ch100_atomic_tests COMMAND ch100_atomic_tests)

add_executable(ch100_atomic_benchmark 100-atomic-data-structures-cas/benchmarks/atomic_structures_benchmark.c)
target_link_libraries(ch100_atomic_benchmark PRIVATE ch100_atomic Threads::Threads)

# Chapter 101
add_library(ch101_concurrent_hash_map
    101-concurrent-hash-map/src/concurrent_hash_map.c
)
target_include_directories(ch101_concurrent_hash_map PUBLIC 101-concurrent-hash-map/src)
target_link_libraries(ch101_concurrent_hash_map PUBLIC Threads::Threads)

add_executable(ch101_concurrent_hash_map_demo 101-concurrent-hash-map/examples/concurrent_hash_map_demo.c)
target_link_libraries(ch101_concurrent_hash_map_demo PRIVATE ch101_concurrent_hash_map)
add_test(NAME ch101_concurrent_hash_map_demo_smoke COMMAND ch101_concurrent_hash_map_demo)

add_executable(ch101_concurrent_hash_map_tests 101-concurrent-hash-map/tests/test_concurrent_hash_map.c)
target_link_libraries(ch101_concurrent_hash_map_tests PRIVATE ch101_concurrent_hash_map)
add_test(NAME ch101_concurrent_hash_map_tests COMMAND ch101_concurrent_hash_map_tests)

add_executable(ch101_concurrent_hash_map_benchmark 101-concurrent-hash-map/benchmarks/concurrent_hash_map_benchmark.c)
target_link_libraries(ch101_concurrent_hash_map_benchmark PRIVATE ch101_concurrent_hash_map)

# Chapter 102
add_library(ch102_concurrent_ring
    102-concurrent-queue-ring-buffer/src/concurrent_mpmc_ring.c
)
target_include_directories(ch102_concurrent_ring PUBLIC 102-concurrent-queue-ring-buffer/src)
target_link_libraries(ch102_concurrent_ring PUBLIC Threads::Threads)

add_executable(ch102_concurrent_ring_demo 102-concurrent-queue-ring-buffer/examples/concurrent_mpmc_ring_demo.c)
target_link_libraries(ch102_concurrent_ring_demo PRIVATE ch102_concurrent_ring)
add_test(NAME ch102_concurrent_ring_demo_smoke COMMAND ch102_concurrent_ring_demo)

add_executable(ch102_concurrent_ring_tests 102-concurrent-queue-ring-buffer/tests/test_concurrent_mpmc_ring.c)
target_link_libraries(ch102_concurrent_ring_tests PRIVATE ch102_concurrent_ring)
add_test(NAME ch102_concurrent_ring_tests COMMAND ch102_concurrent_ring_tests)

add_executable(ch102_concurrent_ring_benchmark 102-concurrent-queue-ring-buffer/benchmarks/concurrent_mpmc_ring_benchmark.c)
target_link_libraries(ch102_concurrent_ring_benchmark PRIVATE ch102_concurrent_ring)


# Chapter 103
add_library(ch103_object_pool
    103-memory-pool-object-pool/src/object_pool.c
)
target_include_directories(ch103_object_pool PUBLIC 103-memory-pool-object-pool/src)

add_executable(ch103_object_pool_demo 103-memory-pool-object-pool/examples/object_pool_demo.c)
target_link_libraries(ch103_object_pool_demo PRIVATE ch103_object_pool)
add_test(NAME ch103_object_pool_demo_smoke COMMAND ch103_object_pool_demo)

add_executable(ch103_object_pool_tests 103-memory-pool-object-pool/tests/test_object_pool.c)
target_link_libraries(ch103_object_pool_tests PRIVATE ch103_object_pool)
add_test(NAME ch103_object_pool_tests COMMAND ch103_object_pool_tests)

add_executable(ch103_object_pool_benchmark 103-memory-pool-object-pool/benchmarks/object_pool_benchmark.c)
target_link_libraries(ch103_object_pool_benchmark PRIVATE ch103_object_pool)

# Chapter 104
add_library(ch104_arena
    104-arena-allocator/src/arena_allocator.c
)
target_include_directories(ch104_arena PUBLIC 104-arena-allocator/src)

add_executable(ch104_arena_demo 104-arena-allocator/examples/arena_allocator_demo.c)
target_link_libraries(ch104_arena_demo PRIVATE ch104_arena)
add_test(NAME ch104_arena_demo_smoke COMMAND ch104_arena_demo)

add_executable(ch104_arena_tests 104-arena-allocator/tests/test_arena_allocator.c)
target_link_libraries(ch104_arena_tests PRIVATE ch104_arena)
add_test(NAME ch104_arena_tests COMMAND ch104_arena_tests)

add_executable(ch104_arena_benchmark 104-arena-allocator/benchmarks/arena_allocator_benchmark.c)
target_link_libraries(ch104_arena_benchmark PRIVATE ch104_arena)

# Chapter 105
add_library(ch105_slab
    105-slab-allocator-concepts/src/slab_allocator.c
)
target_include_directories(ch105_slab PUBLIC 105-slab-allocator-concepts/src)

add_executable(ch105_slab_demo 105-slab-allocator-concepts/examples/slab_allocator_demo.c)
target_link_libraries(ch105_slab_demo PRIVATE ch105_slab)
add_test(NAME ch105_slab_demo_smoke COMMAND ch105_slab_demo)

add_executable(ch105_slab_tests 105-slab-allocator-concepts/tests/test_slab_allocator.c)
target_link_libraries(ch105_slab_tests PRIVATE ch105_slab)
add_test(NAME ch105_slab_tests COMMAND ch105_slab_tests)

add_executable(ch105_slab_benchmark 105-slab-allocator-concepts/benchmarks/slab_allocator_benchmark.c)
target_link_libraries(ch105_slab_benchmark PRIVATE ch105_slab)


# Chapter 106
add_library(ch106_free_list
    106-free-list/src/free_list_allocator.c
)
target_include_directories(ch106_free_list PUBLIC 106-free-list/src)

add_executable(ch106_free_list_demo 106-free-list/examples/free_list_demo.c)
target_link_libraries(ch106_free_list_demo PRIVATE ch106_free_list)
add_test(NAME ch106_free_list_demo_smoke COMMAND ch106_free_list_demo)

add_executable(ch106_free_list_tests 106-free-list/tests/test_free_list_allocator.c)
target_link_libraries(ch106_free_list_tests PRIVATE ch106_free_list)
add_test(NAME ch106_free_list_tests COMMAND ch106_free_list_tests)

add_executable(ch106_free_list_benchmark 106-free-list/benchmarks/free_list_benchmark.c)
target_link_libraries(ch106_free_list_benchmark PRIVATE ch106_free_list)

# Chapter 107
add_library(ch107_gc_graph
    107-garbage-collected-data-structures/src/gc_graph.c
)
target_include_directories(ch107_gc_graph PUBLIC 107-garbage-collected-data-structures/src)

add_executable(ch107_gc_graph_demo 107-garbage-collected-data-structures/examples/gc_graph_demo.c)
target_link_libraries(ch107_gc_graph_demo PRIVATE ch107_gc_graph)
add_test(NAME ch107_gc_graph_demo_smoke COMMAND ch107_gc_graph_demo)

add_executable(ch107_gc_graph_tests 107-garbage-collected-data-structures/tests/test_gc_graph.c)
target_link_libraries(ch107_gc_graph_tests PRIVATE ch107_gc_graph)
add_test(NAME ch107_gc_graph_tests COMMAND ch107_gc_graph_tests)

add_executable(ch107_gc_graph_benchmark 107-garbage-collected-data-structures/benchmarks/gc_graph_benchmark.c)
target_link_libraries(ch107_gc_graph_benchmark PRIVATE ch107_gc_graph)

# Chapter 108
add_library(ch108_cache_aware
    108-cache-aware-data-structures/src/cache_aware_matrix.c
)
target_include_directories(ch108_cache_aware PUBLIC 108-cache-aware-data-structures/src)

add_executable(ch108_cache_aware_demo 108-cache-aware-data-structures/examples/cache_aware_matrix_demo.c)
target_link_libraries(ch108_cache_aware_demo PRIVATE ch108_cache_aware)
add_test(NAME ch108_cache_aware_demo_smoke COMMAND ch108_cache_aware_demo)

add_executable(ch108_cache_aware_tests 108-cache-aware-data-structures/tests/test_cache_aware_matrix.c)
target_link_libraries(ch108_cache_aware_tests PRIVATE ch108_cache_aware)
add_test(NAME ch108_cache_aware_tests COMMAND ch108_cache_aware_tests)

add_executable(ch108_cache_aware_benchmark 108-cache-aware-data-structures/benchmarks/cache_aware_matrix_benchmark.c)
target_link_libraries(ch108_cache_aware_benchmark PRIVATE ch108_cache_aware)


# Chapter 109
add_library(ch109_cache_oblivious
    109-cache-oblivious-data-structures/src/cache_oblivious_matrix.c
)
target_include_directories(ch109_cache_oblivious PUBLIC 109-cache-oblivious-data-structures/src)

add_executable(ch109_cache_oblivious_demo 109-cache-oblivious-data-structures/examples/cache_oblivious_matrix_demo.c)
target_link_libraries(ch109_cache_oblivious_demo PRIVATE ch109_cache_oblivious)
add_test(NAME ch109_cache_oblivious_demo_smoke COMMAND ch109_cache_oblivious_demo)

add_executable(ch109_cache_oblivious_tests 109-cache-oblivious-data-structures/tests/test_cache_oblivious_matrix.c)
target_link_libraries(ch109_cache_oblivious_tests PRIVATE ch109_cache_oblivious)
add_test(NAME ch109_cache_oblivious_tests COMMAND ch109_cache_oblivious_tests)

add_executable(ch109_cache_oblivious_benchmark 109-cache-oblivious-data-structures/benchmarks/cache_oblivious_matrix_benchmark.c)
target_link_libraries(ch109_cache_oblivious_benchmark PRIVATE ch109_cache_oblivious)

# Chapter 110
add_library(ch110_external_memory
    110-external-memory-data-structures/src/external_memory_set.c
)
target_include_directories(ch110_external_memory PUBLIC 110-external-memory-data-structures/src)

add_executable(ch110_external_memory_demo 110-external-memory-data-structures/examples/external_memory_set_demo.c)
target_link_libraries(ch110_external_memory_demo PRIVATE ch110_external_memory)
add_test(NAME ch110_external_memory_demo_smoke COMMAND ch110_external_memory_demo)

add_executable(ch110_external_memory_tests 110-external-memory-data-structures/tests/test_external_memory_set.c)
target_link_libraries(ch110_external_memory_tests PRIVATE ch110_external_memory)
add_test(NAME ch110_external_memory_tests COMMAND ch110_external_memory_tests)

add_executable(ch110_external_memory_benchmark 110-external-memory-data-structures/benchmarks/external_memory_set_benchmark.c)
target_link_libraries(ch110_external_memory_benchmark PRIVATE ch110_external_memory)

# Chapter 111
add_library(ch111_disk_index
    111-disk-based-data-structures/src/disk_sorted_index.c
)
target_include_directories(ch111_disk_index PUBLIC 111-disk-based-data-structures/src)

add_executable(ch111_disk_index_demo 111-disk-based-data-structures/examples/disk_sorted_index_demo.c)
target_link_libraries(ch111_disk_index_demo PRIVATE ch111_disk_index)
add_test(NAME ch111_disk_index_demo_smoke COMMAND ch111_disk_index_demo)

add_executable(ch111_disk_index_tests 111-disk-based-data-structures/tests/test_disk_sorted_index.c)
target_link_libraries(ch111_disk_index_tests PRIVATE ch111_disk_index)
add_test(NAME ch111_disk_index_tests COMMAND ch111_disk_index_tests)

add_executable(ch111_disk_index_benchmark 111-disk-based-data-structures/benchmarks/disk_sorted_index_benchmark.c)
target_link_libraries(ch111_disk_index_benchmark PRIVATE ch111_disk_index)


# Chapter 112
add_library(ch112_db_index
    112-database-index-structures/src/db_index.c
)
target_include_directories(ch112_db_index PUBLIC 112-database-index-structures/src)

add_executable(ch112_db_index_demo 112-database-index-structures/examples/db_index_demo.c)
target_link_libraries(ch112_db_index_demo PRIVATE ch112_db_index)
add_test(NAME ch112_db_index_demo_smoke COMMAND ch112_db_index_demo)

add_executable(ch112_db_index_tests 112-database-index-structures/tests/test_db_index.c)
target_link_libraries(ch112_db_index_tests PRIVATE ch112_db_index)
add_test(NAME ch112_db_index_tests COMMAND ch112_db_index_tests)

add_executable(ch112_db_index_benchmark 112-database-index-structures/benchmarks/db_index_benchmark.c)
target_link_libraries(ch112_db_index_benchmark PRIVATE ch112_db_index)

# Chapter 113
add_library(ch113_mini_fs
    113-file-system-data-structures/src/mini_fs.c
)
target_include_directories(ch113_mini_fs PUBLIC 113-file-system-data-structures/src)

add_executable(ch113_mini_fs_demo 113-file-system-data-structures/examples/mini_fs_demo.c)
target_link_libraries(ch113_mini_fs_demo PRIVATE ch113_mini_fs)
add_test(NAME ch113_mini_fs_demo_smoke COMMAND ch113_mini_fs_demo)

add_executable(ch113_mini_fs_tests 113-file-system-data-structures/tests/test_mini_fs.c)
target_link_libraries(ch113_mini_fs_tests PRIVATE ch113_mini_fs)
add_test(NAME ch113_mini_fs_tests COMMAND ch113_mini_fs_tests)

add_executable(ch113_mini_fs_benchmark 113-file-system-data-structures/benchmarks/mini_fs_benchmark.c)
target_link_libraries(ch113_mini_fs_benchmark PRIVATE ch113_mini_fs)

# Chapter 114
add_library(ch114_compiler_structures
    114-compiler-data-structures/src/compiler_structures.c
)
target_include_directories(ch114_compiler_structures PUBLIC 114-compiler-data-structures/src)

add_executable(ch114_compiler_structures_demo 114-compiler-data-structures/examples/compiler_structures_demo.c)
target_link_libraries(ch114_compiler_structures_demo PRIVATE ch114_compiler_structures)
add_test(NAME ch114_compiler_structures_demo_smoke COMMAND ch114_compiler_structures_demo)

add_executable(ch114_compiler_structures_tests 114-compiler-data-structures/tests/test_compiler_structures.c)
target_link_libraries(ch114_compiler_structures_tests PRIVATE ch114_compiler_structures)
add_test(NAME ch114_compiler_structures_tests COMMAND ch114_compiler_structures_tests)

add_executable(ch114_compiler_structures_benchmark 114-compiler-data-structures/benchmarks/compiler_structures_benchmark.c)
target_link_libraries(ch114_compiler_structures_benchmark PRIVATE ch114_compiler_structures)


# Chapter 115
add_library(ch115_os_structures
    115-operating-system-data-structures/src/os_structures.c
)
target_include_directories(ch115_os_structures PUBLIC 115-operating-system-data-structures/src)

add_executable(ch115_os_structures_demo 115-operating-system-data-structures/examples/os_structures_demo.c)
target_link_libraries(ch115_os_structures_demo PRIVATE ch115_os_structures)
add_test(NAME ch115_os_structures_demo_smoke COMMAND ch115_os_structures_demo)

add_executable(ch115_os_structures_tests 115-operating-system-data-structures/tests/test_os_structures.c)
target_link_libraries(ch115_os_structures_tests PRIVATE ch115_os_structures)
add_test(NAME ch115_os_structures_tests COMMAND ch115_os_structures_tests)

add_executable(ch115_os_structures_benchmark 115-operating-system-data-structures/benchmarks/os_structures_benchmark.c)
target_link_libraries(ch115_os_structures_benchmark PRIVATE ch115_os_structures)

# Chapter 116
add_library(ch116_network_structures
    116-networking-data-structures/src/network_structures.c
)
target_include_directories(ch116_network_structures PUBLIC 116-networking-data-structures/src)

add_executable(ch116_network_structures_demo 116-networking-data-structures/examples/network_structures_demo.c)
target_link_libraries(ch116_network_structures_demo PRIVATE ch116_network_structures)
add_test(NAME ch116_network_structures_demo_smoke COMMAND ch116_network_structures_demo)

add_executable(ch116_network_structures_tests 116-networking-data-structures/tests/test_network_structures.c)
target_link_libraries(ch116_network_structures_tests PRIVATE ch116_network_structures)
add_test(NAME ch116_network_structures_tests COMMAND ch116_network_structures_tests)

add_executable(ch116_network_structures_benchmark 116-networking-data-structures/benchmarks/network_structures_benchmark.c)
target_link_libraries(ch116_network_structures_benchmark PRIVATE ch116_network_structures)

# Chapter 117
add_library(ch117_ml_structures
    117-ai-ml-data-structures/src/ml_structures.c
)
target_include_directories(ch117_ml_structures PUBLIC 117-ai-ml-data-structures/src)

add_executable(ch117_ml_structures_demo 117-ai-ml-data-structures/examples/ml_structures_demo.c)
target_link_libraries(ch117_ml_structures_demo PRIVATE ch117_ml_structures)
add_test(NAME ch117_ml_structures_demo_smoke COMMAND ch117_ml_structures_demo)

add_executable(ch117_ml_structures_tests 117-ai-ml-data-structures/tests/test_ml_structures.c)
target_link_libraries(ch117_ml_structures_tests PRIVATE ch117_ml_structures)
add_test(NAME ch117_ml_structures_tests COMMAND ch117_ml_structures_tests)

add_executable(ch117_ml_structures_benchmark 117-ai-ml-data-structures/benchmarks/ml_structures_benchmark.c)
target_link_libraries(ch117_ml_structures_benchmark PRIVATE ch117_ml_structures)


# Chapter 118
add_library(ch118_vector_index
    118-vector-search-structures/src/vector_index.c
)
target_include_directories(ch118_vector_index PUBLIC 118-vector-search-structures/src)
target_link_libraries(ch118_vector_index PUBLIC m)

add_executable(ch118_vector_index_demo 118-vector-search-structures/examples/vector_index_demo.c)
target_link_libraries(ch118_vector_index_demo PRIVATE ch118_vector_index)
add_test(NAME ch118_vector_index_demo_smoke COMMAND ch118_vector_index_demo)

add_executable(ch118_vector_index_tests 118-vector-search-structures/tests/test_vector_index.c)
target_link_libraries(ch118_vector_index_tests PRIVATE ch118_vector_index)
add_test(NAME ch118_vector_index_tests COMMAND ch118_vector_index_tests)

add_executable(ch118_vector_index_benchmark 118-vector-search-structures/benchmarks/vector_index_benchmark.c)
target_link_libraries(ch118_vector_index_benchmark PRIVATE ch118_vector_index)

# Chapter 119
add_library(ch119_game_structures
    119-game-data-structures/src/game_structures.c
)
target_include_directories(ch119_game_structures PUBLIC 119-game-data-structures/src)
target_link_libraries(ch119_game_structures PUBLIC m)

add_executable(ch119_game_structures_demo 119-game-data-structures/examples/game_structures_demo.c)
target_link_libraries(ch119_game_structures_demo PRIVATE ch119_game_structures)
add_test(NAME ch119_game_structures_demo_smoke COMMAND ch119_game_structures_demo)

add_executable(ch119_game_structures_tests 119-game-data-structures/tests/test_game_structures.c)
target_link_libraries(ch119_game_structures_tests PRIVATE ch119_game_structures)
add_test(NAME ch119_game_structures_tests COMMAND ch119_game_structures_tests)

add_executable(ch119_game_structures_benchmark 119-game-data-structures/benchmarks/game_structures_benchmark.c)
target_link_libraries(ch119_game_structures_benchmark PRIVATE ch119_game_structures)

# Chapter 120
add_library(ch120_merkle_tree
    120-merkle-tree/src/merkle_tree.c
)
target_include_directories(ch120_merkle_tree PUBLIC 120-merkle-tree/src)

add_executable(ch120_merkle_tree_demo 120-merkle-tree/examples/merkle_tree_demo.c)
target_link_libraries(ch120_merkle_tree_demo PRIVATE ch120_merkle_tree)
add_test(NAME ch120_merkle_tree_demo_smoke COMMAND ch120_merkle_tree_demo)

add_executable(ch120_merkle_tree_tests 120-merkle-tree/tests/test_merkle_tree.c)
target_link_libraries(ch120_merkle_tree_tests PRIVATE ch120_merkle_tree)
add_test(NAME ch120_merkle_tree_tests COMMAND ch120_merkle_tree_tests)

add_executable(ch120_merkle_tree_benchmark 120-merkle-tree/benchmarks/merkle_tree_benchmark.c)
target_link_libraries(ch120_merkle_tree_benchmark PRIVATE ch120_merkle_tree)
