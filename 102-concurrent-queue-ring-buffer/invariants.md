# Invariants

1. capacity >= 2 and power of two.
2. mask = capacity - 1.
3. every cell has atomic sequence + one payload slot.
4. producer writes payload before release-publishing ready sequence.
5. consumer reads payload only after acquire-ready observation.
6. consumer advances sequence by capacity before future producer reuse.
7. enqueue/dequeue positions do not wrap during teaching-object lifetime.
8. quiescent enqueue_pos >= dequeue_pos.
9. quiescent position difference <= capacity.
10. destruction occurs only after workers stop.
