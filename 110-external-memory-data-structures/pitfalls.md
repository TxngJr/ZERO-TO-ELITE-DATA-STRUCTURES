# Common Mistakes

- วิเคราะห์แต่ comparisons แล้วไม่คิด block transfers.
- binary-search ทั้ง data แล้วนับเป็นหนึ่ง I/O โดยไม่มี model.
- directory ใหญ่จนจริง ๆ ไม่ fit RAM แต่ยัง assume free.
- range scan กระโดด random blocks แทน sequential blocks.
- input duplicate/unsorted แล้ว directory invariants พัง.
- confuse logical I/O counter กับ actual OS disk I/O.
