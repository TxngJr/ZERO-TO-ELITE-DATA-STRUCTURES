# Lab 055 — Split the Plane

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch055_quadtree_tests

Tasks:
1. Create domain [-100,100)².
2. Insert clustered points.
3. Observe node count as buckets split.
4. Insert duplicate coordinates with different IDs.
5. Query a small rectangle.
6. Query whole domain.
7. Compare with a naive point list.
