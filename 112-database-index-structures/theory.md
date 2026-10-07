# Theory — Database Index Structures

Database index คือ auxiliary structure ที่แลก write/storage cost กับ read access path ที่ดีขึ้น.

แนวคิดสำคัญ:
- clustered vs secondary index
- unique vs non-unique index
- composite ordering
- covering index
- range scan
- selectivity
- lookup + row fetch

บทนี้ใช้ static sorted arrays เพื่อโฟกัส semantics ของ primary/secondary index โดยไม่ซ้ำ implementation B+ Tree จาก Chapter 033.

Production database engine อาจใช้ B+ Tree, LSM Tree, hash index, bitmap index หรือ specialized indexes ตาม workload.
