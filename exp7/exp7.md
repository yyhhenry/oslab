# exp7

```sh
# 查看我们实现的psinfo
cat /proc/psinfo

# 添加一个不断pause的进程和一个死循环的进程
./do_exp7.sh
./task_pause &
./task_busy &

# 查看我们实现的psinfo
cat /proc/psinfo
```
