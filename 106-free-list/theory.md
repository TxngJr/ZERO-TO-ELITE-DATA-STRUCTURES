# Theory — Free Lists

Free list คือ representation ของพื้นที่ว่างเป็น extents/blocks ที่สามารถค้นหาเพื่อสนอง allocation request.

Policies ที่พบบ่อย:
- first fit
- next fit
- best fit
- segregated free lists

ปัญหาหลัก:
- splitting
- coalescing
- alignment
- metadata placement
- external fragmentation

บทนี้ใช้ first-fit + address-sorted free list เพราะ proof ของ coalescing ชัดเจน. Production allocator อาจใช้ boundary tags, bins, trees หรือ size-segregated lists เพื่อปรับ complexity.
