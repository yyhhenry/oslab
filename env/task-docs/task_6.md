# 字符显示的控制

## 实验目的

- 加深对操作系统设备管理基本原理的认识，实践设备端口、键盘中断、扫描码、显存等概念；
- 通过实践掌握Linux对键盘终端和显示器终端的处理过程；
- 能修改Linux 0.11的终端设备处理代码，控制键盘处理和字符显示。

## 实验内容

本实验的基本内容就是修改Linux 0.11的终端设备处理代码，掌握如何能对键盘处理和字符显示进行适当的控制。

- F12控制input_mask开关，输入的字符回显变为*，而不影响实际输入。
- F11控制output_mask开关，输出的字符显示为*。
- 以上只对满足isalnum(c)的字符生效

## 提示

修改`kernel/chr_drv/keyboard_src/keyboard.S`，找到关于`F12`和`F11`的入口，从`func`改为`toggle_input_mask`和`toggle_output_mask`，然后在`kernel/chr_drv/tty_io.c`中实现这两个函数。

额外修改`tty_write`和`copy_to_cooked`函数中所有用到`PUTCH(c, tty->write_q)`的地方，根据`input_mask`和`output_mask`的状态，决定是否输出`*`。

使用`cat -`做简单测试，详见`exp6/`目录下的说明。
