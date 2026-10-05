# Visual Model — SAM Extension

Before appending c:

    last ----suffix links----> root

Create:

    cur.max_len = last.max_len + 1

Add missing c transitions while walking suffix links.

If existing transition p -c-> q violates:

    p.max_len + 1 == q.max_len

create clone:

          +--> q
    ... c |
          +--> clone

Redirect selected transitions to clone.

Then both q and cur suffix-link to clone.
