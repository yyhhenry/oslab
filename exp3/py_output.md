# Output

## 使用朴素时间片算法

```c
/* Line 93 in kernel/sched.c */
#define USE_NAIIVE_TIME_STAMP 1
```

```sh
./exp3/setup.sh
./run.sh

# In bochs
./do_exp3.sh # And check the Parent PID
sync
# Exit bochs

./mount.sh
cat exp3/default_pid.txt > exp3/result_naive.log
cat env/hdc/var/process.log >> exp3/result_naive.log
python3 resolve_exp3_result.py exp3/result_naive.log
rm exp3/result_naive.log # (Optional)
```

```txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 63, Run Time: 3656
PID: 14, Wait Time: 769, Run Time: 2949
PID: 15, Wait Time: 1436, Run Time: 2282
PID: 16, Wait Time: 1562, Run Time: 2155
PID: 17, Wait Time: 1507, Run Time: 2208
Average Wait Time: 10.7s, Average Run Time: 26.5s
Throughput: 8.06018 sub_task/min
```

## 使用Linux-0.11标准算法

```c
/* Line 93 in kernel/sched.c */
#define USE_NAIIVE_TIME_STAMP 0
```

```sh
./exp3/setup.sh
./run.sh

# In bochs
./do_exp3.sh # And check the Parent PID
sync
# Exit bochs

./mount.sh
cat exp3/default_pid.txt > exp3/result.log
cat env/hdc/var/process.log >> exp3/result.log
python3 resolve_exp3_result.py exp3/result.log
rm exp3/result.log # (Optional)
```

```txt
Parent PID: 12, Child PIDs: [13, 14, 15, 16, 17]
PID: 13, Wait Time: 64, Run Time: 3656
PID: 14, Wait Time: 635, Run Time: 3084
PID: 15, Wait Time: 1227, Run Time: 2491
PID: 16, Wait Time: 1518, Run Time: 2199
PID: 17, Wait Time: 1511, Run Time: 2205
Average Wait Time: 9.91s, Average Run Time: 27.3s
Throughput: 8.06018 sub_task/min
```
