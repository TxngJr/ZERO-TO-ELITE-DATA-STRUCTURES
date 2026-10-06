# Hands-On Lab — Branch Historical Versions

## Goal
พิสูจน์ด้วย test ว่า version เก่าไม่เปลี่ยนและแตก branch ใหม่ได้.

## Task
1. สร้าง `v0`.
2. insert 10 -> `v1`.
3. insert 20 จาก `v1` -> `v2`.
4. insert 5 **จาก v1** -> `v3`.
5. ตรวจว่า v2 ไม่มี 5 และ v3 ไม่มี 20.
6. วัด arena nodes.

## Commands

```bash
cmake -S . -B build
cmake --build build --target ch092_persistent_tests ch092_persistent_benchmark
ctest --test-dir build -R ch092 --output-on-failure
./build/ch092_persistent_benchmark
```

## Debugging Task
ทดลองแก้ implementation ให้ mutate `root->left` โดยถอด `const` แล้วดู historical test ล้มเหลว จากนั้นอธิบาย aliasing.

## Extension
ทำ AVL path-copying และเปรียบเทียบ height/worst-case.
