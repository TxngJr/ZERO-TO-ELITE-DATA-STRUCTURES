# Chapter 029 — Trie / Prefix Tree

## Goal

Trie เก็บ keys ตามลำดับ symbols แทนการเปรียบเทียบ key ทั้งก้อนหรือ hash ทั้งก้อน.

บทนี้สร้าง **Byte Trie**:
- alphabet = byte values 0..255
- key = byte sequence + explicit length
- child edges เก็บแบบ sparse sorted linked list
- รองรับ empty key
- exact lookup
- prefix lookup/count
- deletion + pruning
- lexicographic traversal

หลังจบบทนี้คุณควร:
- เข้าใจ prefix sharing
- terminal flag vs node existence
- insert/search/delete
- prefix query
- lexicographic DFS
- sparse vs dense child representation
- complexity in terms of key length
- memory trade-offs
- byte alphabet vs Unicode code points

## 1. Trie Mental Model

Keys:

    car
    cat
    dog

share paths:

    root
     ├─ c
     │   └─ a
     │      ├─ r*
     │      └─ t*
     └─ d
         └─ o
             └─ g*

* means terminal word.

Node existence does not imply a complete key.
Terminal flag does.

## 2. Prefix Sharing

For keys:

    app
    apple
    application

prefix "app" path is stored once.

Trie can answer:

    has_prefix("app")

without scanning unrelated keys.

## 3. Exact Key vs Prefix

If path "ca" exists because "car" exists:

    contains("ca") = false
    has_prefix("ca") = true

unless "ca" itself was inserted and terminal=true.

## 4. Empty Key

Root itself can be terminal.

Therefore zero-length key is valid:

    insert(NULL,0)

API only requires bytes non-NULL when length>0.

This makes the data model mathematically complete for byte sequences.

## 5. Sparse Child Representation

Dense byte trie could store:

    child[256]

per node.

Lookup per symbol:
    O(1)

but every node pays 256 pointers.

This chapter instead stores only existing edges in a sorted linked list:

    Edge(label, child, next)

Advantages:
- memory proportional to existing branching
- lexicographic child order natural

Cost:
- child lookup scans degree edges
- degree <= 256 for byte alphabet

Because alphabet size is fixed at 256, asymptotically lookup remains O(L), but constants can differ significantly.

For generalized alphabets, degree factor should be stated explicitly.

## 6. Insert

For every byte:
1. find sorted edge
2. create missing child/edge
3. follow child
4. after final byte set terminal=true

If already terminal:
    duplicate insert returns false

size counts stored keys, not nodes.

## 7. Exact Search

Follow every byte edge.

After L symbols:
return node.terminal.

Complexity for fixed byte alphabet:

    O(L)

More explicit sparse bound:

    O(sum scanned child degrees)
    <= O(256L)

## 8. Prefix Search

Follow prefix bytes.

If path exists:
    has_prefix = true

No terminal requirement.

Empty prefix exists for every trie, including empty trie, because root exists.

## 9. Count Prefix

Locate prefix node then count terminal nodes in its subtree.

This implementation computes count by traversal:

    O(L + S)

where S = nodes in matching subtree.

A production trie can cache subtree terminal counts to make prefix count O(L), at the cost of metadata updates.

## 10. Delete

To remove exact key:
1. follow path recursively
2. clear terminal
3. on unwind prune child nodes that are:
   - non-terminal
   - have no children

Deleting "app" must not destroy "apple".

Deleting "apple" may prune suffix nodes no longer shared.

## 11. Lexicographic Traversal

Edges are sorted by unsigned byte value.

DFS order:
1. emit current key if terminal
2. visit child edges ascending

This yields byte-lexicographic order.

Shorter key comes before its extensions because terminal emission occurs before children.

## 12. Byte Lexicographic Order

This is not locale-aware human language collation.

For UTF-8 text:
- keys are UTF-8 byte sequences
- prefix in bytes works for valid UTF-8 prefixes
- lexicographic byte order is not necessarily user-language dictionary order

Unicode normalization/collation is a separate concern.

## 13. Trie vs Hash Map

Hash Map:
- expected O(L) to hash + expected O(1) table access
- no natural prefix enumeration
- compact key storage depends implementation

Trie:
- O(L) structural navigation
- natural prefix queries
- shared prefixes
- potentially many allocations/pointers

Chapter 150 compares Trie vs Hash Map systematically.

## 14. Trie vs BST

BST over strings:
- comparisons may inspect common prefixes repeatedly
- O(log n) comparisons if balanced

Trie:
- follows each symbol once along path, independent of number of keys for fixed alphabet
- memory can be larger

## 15. Memory Cost

If N nodes and E edges:
tree property gives roughly E=N-1 for nonempty reachable structure.

Sparse representation allocates:
- one TrieNode per prefix node
- one TrieEdge per parent-child relation

Many tiny allocations can cause fragmentation and pointer chasing.

Later:
- Radix Tree compresses unary paths
- Arena allocators reduce allocation overhead
- cache-aware chapters revisit layout.

## 16. Complexity Summary

Let L=key length, S=matched subtree nodes, alphabet sigma=256.

| Operation | Sparse Byte Trie |
|---|---:|
| insert | O(L*sigma), effectively O(L) fixed sigma |
| contains | O(L*sigma), effectively O(L) |
| has-prefix | O(L*sigma), effectively O(L) |
| count-prefix | O(L*sigma + S) |
| remove | O(L*sigma) + pruning |
| visit prefix | O(L*sigma + output subtree) |
| storage | O(nodes + edges) |

## Files

- src/byte_trie.*
- tests/test_byte_trie.c
- examples/trie_demo.c
- benchmarks/prefix_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
