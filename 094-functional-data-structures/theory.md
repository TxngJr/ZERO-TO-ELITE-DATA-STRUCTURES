# Theory

Pure transformation มีรูป `new = f(old,input)` แทนการ mutate old state. Immutable nodes ลด aliasing side effects และทำให้ historical values ใช้ต่อได้.

Two-list queue ใช้สมการ `Q = front ++ reverse(rear)`. Implementation นี้ strict: reverse rear เมื่อ front ว่าง. Functional literature ยังมี lazy/real-time queues ที่ schedule work เพื่อปรับ worst-case latency.

Persistence เกิดขึ้นตามธรรมชาติจาก immutable links แต่บทนี้เน้น programming model และ composition มากกว่า version taxonomy ของ Chapter 092.
