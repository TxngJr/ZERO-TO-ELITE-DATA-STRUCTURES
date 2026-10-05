# Visual Model — Arrays

## Static array

index:    0     1     2     3
       +-----+-----+-----+-----+
       | 10  | 20  | 30  | 40  |
       +-----+-----+-----+-----+
       contiguous element storage

## Dynamic vector

control object:

    +------------------+
    | data ------------+------+
    | size = 3         |      |
    | capacity = 8     |      |
    +------------------+      |
                              v
       +----+----+----+----+----+----+----+----+
       | A  | B  | C  |    |    |    |    |    |
       +----+----+----+----+----+----+----+----+
         logical data          spare capacity

## Growth

capacity 4, size 4:

    [A][B][C][D]

push E:

    allocate/grow capacity 8
    copy/move old elements
    [A][B][C][D][E][_][_][_]

old backing address may become invalid

## 2D row-major

    matrix[0]: [a00][a01][a02]
    matrix[1]: [a10][a11][a12]
    matrix[2]: [a20][a21][a22]

physical sequence conceptually follows rows.
