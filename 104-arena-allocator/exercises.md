# Exercises

## Beginner
1. Bump pointer คืออะไร?
2. ทำไม arena free ราย object ไม่ได้?
3. Mark เก็บอะไร?
4. Alignment padding เกิดเมื่อไร?
5. Reset ทำให้ pointer ใด invalid?

## Intermediate
6. เพิ่ม remaining-bytes query.
7. เพิ่ม strdup helper.
8. เพิ่ม memdup helper.
9. วัด padding หลาย alignment.
10. ทำ nested phase ด้วย mark/reset.

## Advanced
11. ออกแบบ spare-block cache.
12. เพิ่ม max-reserved limit.
13. ออกแบบ thread-local arenas.
14. เปรียบเทียบ linked blocks กับ giant reservation.
15. วิเคราะห์ virtual-memory backed arena.

## Implementation
16. เพิ่ม arena_printf helper.
17. เพิ่ม poison-on-reset debug mode.
18. เพิ่ม allocation-failure injection.

## Challenge / Research
19. เปรียบเทียบ region-based memory management.
20. ออกแบบ marker ที่ไม่ expose raw block pointer.
