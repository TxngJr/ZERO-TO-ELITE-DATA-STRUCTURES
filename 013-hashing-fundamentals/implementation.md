# Implementation — Hash Functions

## Functions

hash_u64_mix
- mix 64-bit integer ผ่าน xor-shifts/multiplications
- ใช้สอน bit diffusion

hash_bytes_fnv1a
- hash arbitrary bytes
- length-aware
- รองรับ embedded zero

hash_c_string_fnv1a
- convenience wrapper สำหรับ null-terminated string

hash_bucket
- reduce 64-bit hash ไป [0,bucket_count)
- reject zero bucket count

## Why length-aware bytes?

Binary byte sequence อาจมี zero byte กลางข้อมูล.

hash_bytes_fnv1a(data,length) กับ C-string wrapper จึงเป็นคนละ contract.

## Security

implementation ในบทนี้มีไว้เพื่อ data-structure education และ deterministic tests ไม่ใช่ password/signature/MAC.
