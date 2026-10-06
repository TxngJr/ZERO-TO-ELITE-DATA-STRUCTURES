# Implementation

Blocks contain up to block_size values. bases[b] stores the first value; offsets[b..b+1] delimit encoded gaps. Decoder validates 64-bit varint width, positive gaps and cumulative overflow.
The reported encoded_bytes field is delta payload only; bases/offset metadata also consumes memory and must be included in real compression accounting.
