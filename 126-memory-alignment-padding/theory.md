# Theory — Memory Alignment & Padding

Alignment คือข้อกำหนดว่า object/fieldควรเริ่มที่ addressที่หารด้วย alignmentลงตัว.

Paddingเกิดได้สองแบบ:
- internal padding ระหว่าง fields
- tail paddingท้าย struct เพื่อให้ array elementถัดไป alignedถูกต้อง

Field orderจึงเปลี่ยน `sizeof` ได้โดยไม่เปลี่ยน logical information.

Over-alignment เช่น 64-byte strideมีประโยชน์ในบางบริบท เช่นแยก hot mutable objectsหรือ cache-line ownership แต่แลกกับ memory footprint.

Wire serializationไม่ควร copy padding bytesโดยตรง; Chapter 125 แยก canonical wire layoutออกจาก Chapter 126 memory layout.
