# os-lab

操作系统实验环境，围绕Linux-0.11内核进行。

内容来自`hit-oslab-linux-20110823.tar.gz`，对环境配置进行了一些调整。

建议使用Ubuntu（可以是虚拟机，但必须是图形化的）进行实验，如果需要使用WSL，请用Codespaces解决无法挂载Minix文件系统的问题。

## Usage

安装`env/gcc-3.4-deb`中的三个deb文件，按照`base, g++, gcc`的顺序。

运行`./run.sh`，这时可能遇到各种缺失so文件的情况，因为这个环境比较古老，你需要手动安装缺失文件的**i386版本**。

善用ChatGPT和搜索引擎，用apt解决缺失文件问题后，就可以正常运行了。

关于挂载，使用`./env/mount.sh`挂载Minix文件系统到`/env/hdc`，启动`./run.sh`后自动卸载。

请勿同时挂载和运行。
