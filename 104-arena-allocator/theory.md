# Theory — Region / Arena Allocation

Arena เปลี่ยนคำถามจาก “object นี้ free เมื่อไร?” เป็น “region นี้หมดอายุเมื่อไร?”.

ข้อดีคือ bump allocation เร็ว, metadata ต่อ object ต่ำ, bulk reclamation และ locality ดี. ข้อเสียคือ free ราย object ไม่ได้ และ pointer ทั้งกลุ่ม invalid หลัง reset.

เหมาะกับ parser AST, compiler phase, request-scoped state, frame scratch memory และ temporary work buffers.
