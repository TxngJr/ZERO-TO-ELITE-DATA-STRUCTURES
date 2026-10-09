# Common Mistakes

- เรียก false sharingว่า data race.
- ใช้ non-atomic shared countersแล้ว benchmark undefined behavior.
- pad objectแต่ baseไม่ได้ cache-line aligned.
- strideไม่ respect atomic alignment.
- assume cache line 64 bytesใน API โดยไม่ parameterize.
- assert padded versionต้องเร็วกว่าทุก run.
- ลืม memory footprint costของ padding.
- ใช้ volatileแทน atomics.
- free aligned pointerแทน raw allocation pointer.
