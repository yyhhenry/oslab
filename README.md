# os-lab

操作系统实验环境，围绕Linux-0.11内核进行。

内容来自`hit-oslab-linux-20110823.tar.gz`，对环境配置进行了一些调整。

建议使用Ubuntu（可以是虚拟机，但必须是图形化的）进行实验，如果需要使用WSL，请用Codespaces解决无法挂载Minix文件系统的问题。

## Usage

安装`env/gcc-3.4-deb`中的三个deb文件，按照`gcc-base, cpp, gcc`的顺序。

运行`make all`编译内核，然后运行`./run.sh`，图形界面正常的话会看到Bochs的界面。

这时可能遇到各种缺失so文件的情况，因为这个环境比较古老，你需要手动安装缺失文件的**i386版本**。

例如，如果提示缺失`libX11.so.6`，你需要`sudo apt install libx11-6:i386`，如果提示缺失`libSM.so.1`，你需要安装`libsm6:i386`。

以此类推，按照缺少的lib的名字，去掉中间的`.so.`，改成全小写，如果数字相连则加上`-`，然后加上`:i386`，就是你需要安装的包名。

善用ChatGPT和搜索引擎，用apt解决缺失文件问题后，就可以正常运行了。

关于挂载，使用`./env/mount.sh`挂载Minix文件系统到`/env/hdc`，启动`./run.sh`后自动卸载。

请勿同时挂载和运行。
