# os-lab

操作系统实验环境，围绕Linux-0.11内核进行。

内容来自`hit-oslab-linux-20110823.tar.gz`，对环境配置进行了一些调整。

建议使用Ubuntu进 22.04行实验，有多种方式可以实现：

- 可以使用物理机，必须包含图形化界面。
- 可以使用虚拟机，必须包含图形化界面。
- 可以使用WSLg（Windows平台安装WSL2得到），配置和连接VSCode比较方便。

## Usage

```sh
# 安装`env/gcc-3.4-deb`中的三个deb文件，按照`gcc-base, cpp, gcc`的顺序。
sudo apt install ./env/gcc-3.4-deb/gcc-3.4-base_3.4.6-6ubuntu2_amd64.deb
sudo apt install ./env/gcc-3.4-deb/cpp-3.4_3.4.6-6ubuntu2_amd64.deb
sudo apt install ./env/gcc-3.4-deb/gcc-3.4_3.4.6-6ubuntu2_amd64.deb

# 将镜像文件放置到根目录
./env/reset_img.sh

# 编译内核 (可选，运行时会自动构建)
## xmake: https://xmake.io/#/zh-cn/guide/installation
xmake build image

# 安装缺失的库
## 如果遇到各种缺失so文件的情况，因为这个环境比较古老，你需要手动安装缺失文件的**i386版本**。
## 例如，如果提示缺失`libX11.so.6`，你需要`libx11-6:i386`，
## 如果提示缺失`libSM.so.1`，你需要安装`libsm6:i386`。
## 以此类推，按照缺少的lib的名字，去掉中间的`.so.`，改成全小写，
## 如果数字相连则加上`-`，然后加上`:i386`，就是你需要安装的包名。
sudo apt install libx11-6:i386 libsm6:i386 g++-multilib

# 运行
./run.sh

# 挂载
## 最好先运行一次`./run.sh`，然后再挂载
## 不要同时挂载和运行，可能损坏文件系统
sudo apt install libguestfs-tools linux-image-generic
# libguestfs-test-tool # 测试是否安装成功
# sudo chmod +r /boot/vmlinuz-5.15.0-122-generic # 修复权限，注意改成你的版本
# sudo chmod 0666 /dev/kvm # 修复权限
# export LIBGUESTFS_BACKEND=direct # 建议不要设置，但如果前面的命令无法解决你的问题，可以尝试设置这个环境变量
./mount.sh # 无需root权限
```
