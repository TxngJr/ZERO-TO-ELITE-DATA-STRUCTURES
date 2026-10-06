# Implementation Notes

Build input must be strictly increasing. Keys are copied into contiguous storage, while block metadata stores:
- start index
- count
- first key
- last key

This deliberately separates:
- CPU work inside RAM directory/block
- modeled block transfers

Counters:
- build increments logical block writes once per produced block
- contains increments one read only when it examines a candidate data block
- range increments one read per examined overlapping block
