# Theory — Knowledge Graph Representation

Knowledge graphมัก represent factsเป็น triples:

```text
(subject, predicate, object)
```

Terms อาจเป็น entities, relations, classes หรือ literals. ระบบจริงมักแยก lexical representation ออกจาก compact numeric IDs เพื่อประหยัด memory และทำ indexes ได้ง่าย.

Triple store ต่างจาก property graphตรงที่ relationถูก representเป็น first-class predicate termใน tuple. Query enginesจึงได้ประโยชน์จากหลาย permutation indexes เช่น SPO, POS และ OSP.

บทนี้เป็น in-memory teaching model ไม่ได้ implement RDF datatype/language tags, SPARQL parser, inference หรือ distributed storage.
