# Implementation

`CowIntVector` แยก handle จาก flexible-array `CowStorage { refs, size, capacity, data[] }`.

Detach allocate/copy ให้สำเร็จก่อน publish. หาก refcount ของ old storage ลดเป็น zero ต้อง free old storage; หากลืมขั้นตอนนี้จะ leak ทุก growth ที่ replace unique backing.

Chapter นี้จงใจใช้ non-atomic refcount เพื่อเชื่อมไป Chapter 096: COW implementation นี้ต้อง external synchronization หากข้าม threads.
