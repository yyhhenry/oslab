# 系统调用

## 实验目的

- 建立对操作系统接口的深入认识
- 掌握系统调用的基本过程
- 能完成系统调用的全面控制

## 实验内容

在Linux 0.11上添加两个系统调用:

```c
int whoami(char* name, unsigned int size);
int iam(const char * name);
```

`whoami`完成的功能是将用户程序中传入的字符串参数`name`的内容拷贝到核心态数据段中。要求`name`的长度不能超过20个字符。`iam`完成的功能是将核心态数据段中的字符串拷贝到用户程序中。

然后实现两个使用这两个系统调用的应用程序，达到如下效果

```txt
$ iam Henry
User name set to Henry
$ whoami
I am Henry
```

## 提示

添加一个系统调用需要完成的工作有：

- 在`include/unistd.h`中找到`__NR_`开头的宏定义列表，并追加新的系统调用号。
- 在`include/linux/sys.h`中找到`extern int sys_`开头的函数声明列表，并追加新的系统调用函数声明，然后在`sys_call_table`数组的末尾依序添加新的系统调用函数名。
- 在`kernel/system_call.s`中找到`nr_system_calls = 72`，并将其值增加1。
- 在`kernel/`下增加一个`.c`文件，实现新的系统调用函数`int sys_xxx(...)`，与`sys.h`中的声明一致。

实现内容时需要注意访问用户空间内存的方式，例如使用`get_fs_byte`和`put_fs_byte`来读写`const char *`类型的参数。

```c
#include <asm/segment.h>
// ...
int sys_whoami(char * /* User space */ name, int size) {
    // ...
    put_fs_byte('\0', name + i);
    // ...
}
// ...
int sys_iam(const char * /* User space */ name) {
    // ...
    c = get_fs_byte(name + i);
    // ...
}
```

使用这个系统调用的用户程序需要补充这些声明，需要与`kernel/`下的`.c`文件的实现对应。`_syscallX`中的`X`表示系统调用的参数个数。

```c
#define __LIBRARY__
#include <unistd.h>

_syscall2(int, whoami, char *, name, int, size);
_syscall1(int, iam, const char *, name);
```

根据`kernel/`下的`.c`文件的实现，提供这些

保证`unistd.h`中存在宏定义：对于本例，你需要`cp $OSLAB_PATH/linux-0.11/include/unistd.h $OSLAB_PATH/env/hdc/usr/include/`，以让系统中写好的`unistd.h`在Linux-0.11内部的gcc中找到。
