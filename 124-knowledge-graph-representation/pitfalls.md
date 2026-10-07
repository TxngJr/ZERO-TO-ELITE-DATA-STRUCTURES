# Common Mistakes

- ใช้ stringซ้ำในทุก tripleแทน term interning.
- ใช้ ID 0 เป็น valid termทั้งที่ query APIใช้เป็น wildcard.
- มีแค่ SPO indexแล้ว predicate/object queriesกลายเป็น full scan.
- sort copied triplesแล้ว mappingกลับ authoritative tableหาย.
- เพิ่ม tripleแล้วลืม invalidate indexes.
- duplicate triple setกับ sorted indexesไม่ตรงกัน.
- เรียก teaching triple store นี้ว่า full RDF/SPARQL engine.
