# 信号量实现和应用

## 实验目的

- 加深对进程同步与互斥概念的认识；
- 掌握信号量的实现原理；
- 在实际操作系统中实践信号量的实现，并加以实际应用。

## 实验内容

在Linux 0.11内核上（Linux 0.11内核中没有定义信号量）实现信号量，并创建相应的系统调用以供用户使用。

在定义了信号量的Linux 0.11操作系统上编写用户程序来演示信号量的作用，该用户程序解决就是传统的生产者—消费者问题。

因为没有实现共享内存，还需要在系统空间提供一个队列，用于生产者和消费者之间的通信。

## 提示

可以提供如下这些系统调用，增加系统调用的方法参考实验2。

```c
_syscall2(int, sem_open, const char *, name, int, value);
_syscall1(int, sem_wait, int, sem);
_syscall1(int, sem_signal, int, sem);
_syscall1(int, sem_close, int, sem);
_syscall1(int, buf_init, int, max_size);
_syscall1(int, buf_push, int, value);
_syscall0(int, buf_pop);
```

可以在`kernel/semaphore.c`中定义信号量结构体和缓冲区结构体

```c
struct Semaphore {
    char name[MAX_SEM_NAME + 1];
    /* Reference count */
    int ref_cnt;
    int value;
    struct task_struct *sem_wait[MAX_SEM_WAIT];
} semaphore_set[NR_SEMS];

struct PCBuffer {
    int max_size;
    int push_ptr;
    int pop_ptr;
    /* One extra element for the buffer */
    int buffer[PC_BUFFER_MAX + 1];
} pc_buffer;
```

可以使用`sleep_on()`和`wake_up()`来实现信号量的等待和唤醒。

可以使用`cli();`和`sti();`来关闭和开启中断，这在本实验的单核系统中是安全的。

对于缓冲区的实现，需要循环队列。

```c
int next_ptr(int ptr)
{
    return (ptr + 1) % (pc_buffer.max_size + 1);
}
```

同样建议给出可反序列化的输出，方便使用Python等脚本语言进行进一步的分析。

为了让VSCode提供比较良好的IntelliSense支持，可以在`xmake.lua`中添加一个不实际用于编译的任务，仅仅让VSCode能够找到所需的头文件。

```lua
target("exp4")
    set_kind("binary")
    set_default(false)
    add_files("exp4/home/*.c")
    add_includedirs("linux-0.11/include")
```
