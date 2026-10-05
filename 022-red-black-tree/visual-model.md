# Visual Model — Red-Black Tree

B = black
R = red

        10B
       /   \
      5R   20R
     / \   / \
    2B 7B 15B 30B

ไม่มี red-red และทุก root→NULL path มี black count สอดคล้องกัน.

## Insert recolor concept

ก่อน:

        10B
       /
      5R
     / \
    2R 7R

เมื่อ uncle 7R:
- 5 -> B
- 7 -> B
- 10 -> R
- แล้ว root ถูกบังคับกลับ B หากเป็น root.

## Line repair

      30B
      /
    20R
    /
  10R

recolor + rotate right at 30:

      20B
     /   \
   10R   30R
