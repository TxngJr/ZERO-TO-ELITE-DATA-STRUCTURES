# Visual Model — Skip List

            +-----------------------+
L3: HEAD -> 10 ---------> 40 ------> 80
            |             |          |
L2: HEAD -> 10 -> 20 ---> 40 -> 60 ->80
            |     |       |     |    |
L1: HEAD -> 10 -> 20 ->30->40->60 ->80
            |     |    |  |  |  |    |
L0: HEAD -> 10 -> 20 ->30->40->50->60->70->80

Every upper-level node also appears in level 0.

Search 50:
- move quickly at upper levels
- drop when next key would exceed 50
- finish at level 0.
