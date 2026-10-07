# Visual Model

```text
Primary array sorted by PK:
[10,s=7,payload] [20,s=4,payload] [30,s=7,payload]

Secondary entries sorted by (SK,PK):
[4,20,row=1] [7,10,row=0] [7,30,row=2]

secondary = 7
lower_bound(7) -> scan until SK != 7
```
