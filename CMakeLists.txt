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
