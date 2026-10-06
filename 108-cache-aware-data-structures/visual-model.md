# Visual Model

สำหรับ tile 2×3:

```text
logical:
a b c | d e f
g h i | j k l
------+------
m n o | p q r

physical:
[a b c g h i][d e f j k l][m n o pad pad pad]...
```

Tile-order traversal อ่านแต่ละ bracket ต่อเนื่องกว่าการกระโดดตาม logical coordinate ที่ไม่สอดคล้องกับ tile storage.
