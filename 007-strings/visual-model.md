# Visual Model — Strings

## Null-terminated

index:   0   1   2   3   4   5
       +---+---+---+---+---+----+
       | h | e | l | l | o | \0 |
       +---+---+---+---+---+----+

logical length = 5

## Length-aware dynamic string

    ByteString
    +----------------+
    | data ----------+----> +---+---+---+----+---+---+
    | length = 3     |      | c | a | t | \0 | ? | ? |
    | capacity = 5   |      +---+---+---+----+---+---+
    +----------------+

capacity counts logical bytes available before terminator slot in this implementation.

## Insert

before:
    [a][b][c][d][\0]

insert XY at index 2:

shift suffix:
    [a][b][c][d][c][d][\0]

write:
    [a][b][X][Y][c][d][\0]

## UTF-8 warning

"é" may occupy multiple bytes in UTF-8.

byte indexes:
    [0][1]

one code point can span both bytes.
