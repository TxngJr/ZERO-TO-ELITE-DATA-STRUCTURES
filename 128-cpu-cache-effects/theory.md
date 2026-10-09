# Theory — CPU Cache Effects

Chapter 127บอกว่า traceมี localityแบบใด. Chapter 128ถามต่อว่า localityนั้นแปลงเป็น cache hit/missอย่างไรภายใต้ cache geometryหนึ่ง.

Cache missเกิดได้จากหลายสาเหตุ:
- compulsory — lineยังไม่เคยเข้ามา
- capacity — working setใหญ่กว่า cache
- conflict — หลาย lines mapไป setเดียวกัน
- replacement effects

Simulatorบทนี้นับ hit/miss/evictionแต่ไม่พยายาม classify 3C missesทั้งหมด.

Associativityลด conflictได้โดยให้แต่ละ setเก็บหลาย lines. LRUเป็น policyสอนง่ายแต่ hardwareจริงอาจใช้ pseudo-LRU หรือ policyอื่น.
