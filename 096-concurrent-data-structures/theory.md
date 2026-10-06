# Theory

Concurrent correctness มีหลายแกน: representation invariant, data-race freedom, object-level linearizability และ progress guarantees.

Mutex ทำ mutual exclusion และ synchronization ordering เมื่อใช้อย่างถูกต้อง. `volatile` ไม่แทน mutex/atomic สำหรับ inter-thread synchronization.

C atomics มี memory orders หลายแบบ; บทนี้ใช้ release/acquire demo เท่านั้น. การเลือก weaker order ต้องพิสูจน์จาก protocol ไม่ใช่เลือกเพราะคิดว่าเร็วกว่า.

Correctness กับ progress คนละเรื่อง: mutex set อาจ linearizable แต่ blocking; lock-free object มี progress guarantee ต่างออกไป.
