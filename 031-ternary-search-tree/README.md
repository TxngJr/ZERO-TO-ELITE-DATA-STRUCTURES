# Chapter 031 — Ternary Search Tree

## Goal

Ternary Search Tree (TST) ผสมแนวคิดของ Trie กับ Binary Search Tree:

- แต่ละ node เก็บ byte หนึ่งค่า
- low child เก็บ byte ที่น้อยกว่า
- equal child ไปยัง byte ถัดไปของ key
- high child เก็บ byte ที่มากกว่า

บทนี้ใช้ explicit byte pointer + length เช่นเดียวกับ Chapters 029–030 เพื่อให้เปรียบเทียบได้ตรง ๆ.

หลังจบบทนี้คุณควร:
- เข้าใจ lo/equal/hi branching
- insert / contains / remove
- prefix search
- lexicographic traversal
- compare TST vs Trie vs Radix Tree
- understand shape sensitivity
- validate local BST ordering per character level
- handle binary keys and empty key

## Node Model

Each node stores:

    unsigned char symbol
    bool terminal
    low
    equal
    high

At the same character depth:
- low subtree symbols < node.symbol
- high subtree symbols > node.symbol
- equal means current symbol matched and we advance input position

## Search

Given byte b at position i:

if b < node.symbol:
    node = low

else if b > node.symbol:
    node = high

else:
    if i is last byte:
        answer = terminal
    else:
        ++i
        node = equal

Unlike ordinary Trie, one character step may traverse several lo/hi comparisons before a match.

## Empty Key

TST node structure needs a symbol, so empty key is represented by a tree-level boolean:

    empty_terminal

This keeps zero-length byte sequence distinct without inventing a sentinel symbol.

## Insert

At each depth:
- compare current input byte with node symbol
- create missing node
- go low/high without consuming input
- go equal and consume input when symbol matches
- mark terminal at final matched symbol

## Prefix Search

A prefix exists when all prefix bytes can be matched.

If matched prefix ends at node symbol:
- has_prefix = true even if terminal=false
- enumeration continues from that node's equal subtree plus the prefix itself if terminal

## Delete

Deletion clears terminal then prunes nodes that have:
- terminal=false
- low=NULL
- equal=NULL
- high=NULL

A TST node with only low/high children cannot be collapsed trivially like a Radix unary path because those children encode alternative symbols at the same depth.

## Lexicographic Traversal

For each node:
1. visit low subtree
2. append node.symbol
3. emit if terminal
4. visit equal subtree with symbol retained in prefix
5. pop symbol
6. visit high subtree

This yields unsigned-byte lexicographic order.

## Complexity

Let L be key length and h_i the search height among alternative symbols at character position i.

Exact search:

    O(sum h_i)

Balanced/typical symbol trees may behave well.

Worst case if symbol comparisons become skewed:

    O(L + M)

where M can include long lo/hi chains; a pathological TST may degrade substantially.

Unlike dense Trie, there is no fixed 256-pointer array per node.

## TST vs Trie

Trie:
- one direct child selection structure per symbol
- sparse representation in Chapter 029 scans edge list
- prefix path explicit

TST:
- exactly three pointers per node
- child alternatives represented as BST-style lo/high
- performance depends on symbol insertion order/shape

## TST vs Radix Tree

Radix:
- compresses runs into labels
- fewer nodes on unary paths

TST:
- one symbol per matched depth
- no edge-label allocation/copy
- simpler character-level mutation

## Practical Uses

- dictionaries
- autocomplete
- prefix lookup
- memory-conscious string indexes with moderate alphabets

As always, benchmark against hash tables, tries and sorted arrays for real workloads.
