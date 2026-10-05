# Visual Model — R-Tree

                Root
          +-------+-------+
          | MBR A | MBR B |
          +---|---+---|---+
              |       |
          +---+--+ +--+---+
          | leaf | | leaf |
          | rects| | rects|
          +------+ +------+

Sibling MBRs may overlap.

A query overlapping both root entries must search both children.
