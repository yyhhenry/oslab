# Output

使用朴素时间片算法

```txt
$ python3 resolve_exp3_result.py exp3/result_naive.txt
exp3/result_naive.txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 7007, Run Time: 1952, Total Time: 8959
PID: 14, Wait Time: 7002, Run Time: 1956, Total Time: 8958
PID: 15, Wait Time: 6919, Run Time: 2039, Total Time: 8958
PID: 16, Wait Time: 6995, Run Time: 1962, Total Time: 8957
PID: 17, Wait Time: 4120, Run Time: 1033, Total Time: 5153
Average Wait Time: 6408.6, Average Run Time: 1788.4, Average Total Time: 8197.0
```

使用Linux-0.11标准算法

```txt
$ python3 resolve_exp3_result.py exp3/result.txt
exp3/result.txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 5669, Run Time: 1515, Total Time: 7184
PID: 14, Wait Time: 5668, Run Time: 1516, Total Time: 7184
PID: 15, Wait Time: 5596, Run Time: 1587, Total Time: 7183
PID: 16, Wait Time: 5651, Run Time: 1531, Total Time: 7182
PID: 17, Wait Time: 4080, Run Time: 1026, Total Time: 5106
Average Wait Time: 5332.8, Average Run Time: 1435.0, Average Total Time: 6767.8
```
