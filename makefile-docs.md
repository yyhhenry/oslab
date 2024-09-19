# Makefile 中文解析

## `Image` 流程

也即 `make all` 的流程

- boot/bootsect
  生成 bootsect
- boot/setup
  生成 setup
- tools/system
  生成 system
  - boot/head.o
    生成 head.o
  - init/main.o
    编译 init/main.c
  - ARCHIVES = kernel/kernel.o mm/mm.o fs/fs.o
    分别进入 kernel, mm, fs 目录编译
  - DRIVERS = kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
    分别进入 blk_drv, chr_drv 目录编译
  - MATH = kernel/math/math.a
    进入 math 目录编译
    由 `ar` 命令整合 `math` 目录下的所有 `.o` 文件为 `math.a`
  - LIBS = lib/lib.a
    进入 lib 目录编译
    由 `ar` 命令整合 `lib` 目录下的所有 `.o` 文件为 `lib.a`

- tools/build 编译 tools/build.c 生成 build
- `cp -f tools/system system.tmp`
  `strip system.tmp`
  `objcopy -O binary -R .note -R .comment system.tmp tools/kernel`
  删除调试信息并生成 tools/kernel 文件
- `tools/build boot/bootsect boot/setup tools/kernel > Image`
  调用 tools/build 生成 Image 文件
- `rm system.tmp`
  `rm tools/kernel -f`
  `sync`
  清理临时文件
