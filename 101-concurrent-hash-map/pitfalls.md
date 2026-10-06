# Common Mistakes

- Lock bucket but read table pointer without protecting resize.
- Rehash while readers still hold old bucket pointers.
- Upgrade read→write lock while holding bucket mutex and create deadlock.
- Publish new bucket_count before new bucket pointer.
- Return put failure after logical insertion succeeded only because optional resize failed.
- Use atomic size and assume linked nodes need no locks.
- Destroy map while workers are active.
- Ignore adversarial collision worst case.
