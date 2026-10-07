# Chapter 125 — Data Structure Serialization

บทนี้สร้าง portable binary serialization format สำหรับ array ของ fixed logical records:

```c
typedef struct {
    uint64_t id;
    int64_t  value;
    uint32_t flags;
} DsRecord;
```

จุดสำคัญคือ **ไม่เขียน raw C struct bytes ลงไฟล์**.

## Canonical Format

Header ขนาด 32 bytes:

```text
0..3   magic = "DSR1"
4..5   version = 1          (big-endian u16)
6..7   header_size = 32     (big-endian u16)
8..15  record_count         (big-endian u64)
16..19 record_size = 20     (big-endian u32)
20..27 payload_size         (big-endian u64)
28..31 payload_crc32        (big-endian u32)
```

แต่ละ record ขนาด 20 bytes:

```text
id     8 bytes big-endian
value  8 bytes canonical bit pattern, big-endian
flags  4 bytes big-endian
```

## Why Not fwrite(struct)?

C struct layoutอาจมี:
- padding
- alignment gaps
- target-dependent endianness
- ABI/compiler differences

ดังนั้น `sizeof(DsRecord)` ไม่ใช่ wire-format contract.

## Validation

Deserializer ตรวจ:
- magic
- version
- header size
- record size
- count × record-size overflow
- payload size consistency
- exact total length (trailing garbage rejected)
- CRC32
- allocation overflow

## Tests

- CRC32 known vector `"123456789"`
- 100,000-record round trip
- deterministic byte-for-byte reserialization
- negative/signed value bit preservation
- corruption rejection
- truncated blob rejection
- bad magic/version/record-size rejection
- trailing-byte rejection
- ASan/UBSan

## Complexity

Serialize/deserialze/validate = Θ(N).
Wire size = 32 + 20N bytes.
