# Output

使用朴素时间片算法

```txt
$ python3 resolve_exp3_result.py exp3/result_naive.txt
exp3/result_naive.txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 4127, Run Time: 1030
PID: 14, Wait Time: 4126, Run Time: 1030
PID: 15, Wait Time: 4126, Run Time: 1030
PID: 16, Wait Time: 4125, Run Time: 1030
PID: 17, Wait Time: 4120, Run Time: 1033
Average Wait Time: 41.2s, Average Run Time: 10.3s
Throughput: 5.81395 sub_task/min
```

使用Linux-0.11标准算法

```txt
$ python3 resolve_exp3_result.py exp3/result.txt      
exp3/result.txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 4090, Run Time: 1020
PID: 14, Wait Time: 4090, Run Time: 1020
PID: 15, Wait Time: 4089, Run Time: 1020
PID: 16, Wait Time: 4088, Run Time: 1020
PID: 17, Wait Time: 4080, Run Time: 1026
Average Wait Time: 40.9s, Average Run Time: 10.2s
Throughput: 5.8674 sub_task/min
```
