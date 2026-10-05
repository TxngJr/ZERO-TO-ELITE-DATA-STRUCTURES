# Lab 020 — Rotate Without Changing Order

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch020_rotation_tests

Tasks:
1. Build 10,5,20,15,30.
2. Record inorder.
3. Rotate left at 10.
4. Verify inorder unchanged and root becomes 20.
5. Rotate right at 20.
6. Construct LR and RL cases.
7. Compare height before/after selected rotations.
