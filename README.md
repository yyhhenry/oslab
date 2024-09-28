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
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install bin86 libx11-6:i386 libsm6:i386 libxpm4:i386 g++-multilib

# 运行
./run.sh

# 挂载
## 最好先运行一次`./run.sh`，然后再挂载
## 不要同时挂载和运行，可能损坏文件系统
sudo apt install libguestfs-tools linux-image-generic
# libguestfs-test-tool # 测试是否安装成功
# sudo usermod -aG kvm $USER # 若要使用kvm，需要这个权限

# 如果libguestfs-tools提示找不到模块，根据/boot和/lib/modules中的情况运行如下命令
# sudo chmod +r /boot/vmlinuz-5.15.0-122-generic # 修复权限，注意改成你的版本
# sudo apt-get install --reinstall linux-modules-5.15.0-122-generic # 修复模块

./mount.sh # 无需root权限
```

## 关于WSL2

### 网络问题

Windows 找到或创建 `C:\Users\$env:USERNAME\.wslconfig`，`$env:USERNAME` 为你的用户名。

```ini
[wsl2]
# kernel 这一行见下文Minix挂载
# kernel=C:\\Windows\\System32\\lxss\\tools\\wsl2-kernel-minix
networkingMode=mirrored
dnsTunneling=true
autoProxy=true
```

### HiDPI

如果你认为界面太小，可以尝试调整HiDPI设置。

Windows 找到或创建 `C:\Users\$env:USERNAME\.wslgconfig`，`$env:USERNAME` 为你的用户名。

注意文件名中的`g`，这是WSLg的配置文件。

```ini
[system-distro-env]
;hi-dpi
WESTON_RDP_HI_DPI_SCALING=true
WESTON_RDP_FRACTIONAL_HI_DPI_SCALING=false
;100 to 500
WESTON_RDP_DEBUG_DESKTOP_SCALING_FACTOR=200
```

### 挂载Minix

本实验中需要挂载的hdc-0.11.img内部使用了Minix文件系统，如果你的WSL2不支持文件系统，尝试编译并替换内核。

首先在 <https://github.com/microsoft/WSL2-Linux-Kernel/releases> 中找到 `uname -r` 对应的版本，下载 `.tar.gz` 文件，解压后放到你的WSL2中。

```sh
# 安装依赖
sudo apt install build-essential flex bison dwarves libssl-dev libelf-dev bc

# 配置选项
make menuconfig
# File systems ->
#   Miscellaneous filesystems ->
#     <*> Minix file-system support
# Save & Exit

# 编译
make all -j$(nproc)

# 将文件 arch/x86/boot/bzImage 
# 拷贝到 Windows 中 C:\Windows\System32\lxss\tools\wsl2-kernel-minix
WIN_USERNAME=$(powershell.exe '$env:USERNAME')
WIN_USERNAME=${WIN_USERNAME//[$'\r\n']}
cp arch/x86/boot/bzImage /mnt/c/Users/$WIN_USERNAME/wsl2-kernel-minix
# On Windows Terminal - PowerShell (Admin)
# mv C:\Users\$env:USERNAME\wsl2-kernel-minix C:\Windows\System32\lxss\tools\wsl2-kernel-minix
# wsl --shutdown
```

随后在 Windows 上 `~\.wslconfig` 中配置：

```ini
[wsl2]
kernel=C:\\Windows\\System32\\lxss\\tools\\wsl2-kernel-minix
```
