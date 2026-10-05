# Lab 056 — Split a Volume

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch056_octree_tests

Tasks:
1. Create [-128,128)^3.
2. Insert points into all 8 octants.
3. Draw first-level boxes.
4. Insert coincident points.
5. Query one sub-box.
6. Query whole volume.
7. Compare with naive 3D scan.
