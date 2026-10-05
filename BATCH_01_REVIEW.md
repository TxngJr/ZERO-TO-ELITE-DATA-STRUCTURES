# Batch 01 — Negative Review and Audit

## Beginner review

Potential failure: ผู้เรียนอาจจำ syntax pointer โดยยังไม่แยก object/address/value
Mitigation: Chapter 001 ใช้ diagram/experiments และ Chapter 002 บังคับดู address ผ่าน GDB

Potential failure: คำว่า stack ชนกันระหว่าง call stack, stack memory และ Stack ADT
Mitigation: Chapter 002 และ 003 แยกสามความหมายชัดเจน

## Computer Science review

Potential failure: ADT ถูกลดรูปเป็น "header file"
Mitigation: Chapter 003 มี algebraic behavior, representation function, invariant และ contract

Potential failure: complexity ถูกอ้างก่อนสอน formal notation
Mitigation: Chapter 003 ระบุเป็น preview เท่านั้น Chapter 004 จะ formalize Big-O/Omega/Theta และ amortized analysis

## Systems review

Potential failure: อธิบาย stack/heap แบบ physical layout ตายตัว
Mitigation: ระบุว่า process model เป็น simplified virtual-address model และ optimizer/ABI มีสิทธิ์เปลี่ยนรายละเอียด

Potential failure: benchmark ถูกตีความเป็นกฎ universal
Mitigation: locality benchmark บังคับให้รันหลายครั้งและห้ามสรุปจากเลขครั้งเดียว

## Memory-safety review

Reviewed:
- malloc failure handling
- realloc temporary-pointer pattern
- overflow checks in IntStack capacity growth
- NULL argument handling
- no use-after-free in examples
- no double free in examples

## API review

IntStack hides representation via opaque type.
Behavioral tests depend on LIFO contract rather than capacity/layout.

## Verification performed

Source equivalents were compiled with:
- GCC/G++ warnings enabled
- AddressSanitizer
- UndefinedBehaviorSanitizer

Verified executables:
- Chapter 001 C example
- Chapter 001 C++ generic example
- Chapter 002 memory example
- Chapter 002 locality benchmark
- Chapter 003 IntStack tests

No sanitizer failure was observed in the verification run.

## Remaining scope

Not a defect:
- formal complexity belongs to Chapter 004
- full recursion analysis belongs to Chapter 005
- full dynamic-array implementation belongs to Chapter 006
- full Stack comparison belongs to Chapter 009

Batch 01 is therefore complete for its declared scope, while later chapters deliberately deepen these previews.
