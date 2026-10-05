# Visual Model — Lazy Tag

Initial interval:

              [0,4)
            sum=10
           /      \
       [0,2)      [2,4)

Apply +3 to [0,4):

              [0,4)
            sum=22
            lazy=3
           /      \
       old data   old data

Children remain physically old.

Later partial descent:

              lazy=0
              /    \
          child+3 child+3

Logical array never changed during push itself.
