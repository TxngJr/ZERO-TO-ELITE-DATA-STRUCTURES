# Lab 022 — Color, Rotate, Validate

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch022_red_black_tests

Tasks:
1. Insert 10,20,30 and trace fix-up.
2. Insert sequence with red uncle and trace recoloring.
3. Record inorder after each insertion.
4. Delete RED leaf.
5. Delete BLACK node and trace sibling cases.
6. Validate after every mutation.
7. Compare sorted insertion height with plain BST and AVL.
