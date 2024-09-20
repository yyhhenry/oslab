set_toolset("cc", "gcc-3.4")
set_arch("i386")

local kernel_cflags = "-m32 -g -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc"

target("boot")
    set_kind("phony")
    after_build(function (target)
        os.vrun("as86 -0 -a -o linux-0.11/boot/bootsect.o linux-0.11/boot/bootsect.s")
        os.vrun("ld86 -0 -s -o linux-0.11/boot/bootsect linux-0.11/boot/bootsect.o")
        
        os.vrun("as86 -0 -a -o linux-0.11/boot/setup.o linux-0.11/boot/setup.s")
        os.vrun("ld86 -0 -s -o linux-0.11/boot/setup linux-0.11/boot/setup.o")
    end)
    after_clean(function (target)
        os.rm("linux-0.11/boot/bootsect.o")
        os.rm("linux-0.11/boot/bootsect")
        os.rm("linux-0.11/boot/setup.o")
        os.rm("linux-0.11/boot/setup")
    end)


target("boot/head")
    set_kind("object")
    add_cflags("-m32 -g")
    add_files("linux-0.11/boot/head.s")
    after_build(function (target)
        local objfiles = table.concat(target:objectfiles(), " ")
        os.vrun("ld -m elf_i386 -r -o linux-0.11/boot/head.o " .. objfiles)
    end)

target("init/main")
    set_kind("static")
    add_includedirs("linux-0.11/include")
    add_cflags("-m32 -g -Wall -O2 -fomit-frame-pointer ")
    add_files("linux-0.11/init/main.c")
    after_build(function (target)
        local objfiles = table.concat(target:objectfiles(), " ")
        os.vrun("ld -m elf_i386 -r -o linux-0.11/init/main.o " .. objfiles)
    end)


target("kernel")
    set_kind("object")
    add_includedirs("linux-0.11/include")
    add_cflags(kernel_cflags)
    add_files("linux-0.11/kernel/*.s")
    add_files("linux-0.11/kernel/*.c")
    after_build(function (target)
        local objfiles = table.concat(target:objectfiles(), " ")
        os.vrun("ld -m elf_i386 -r -o linux-0.11/kernel/kernel.o " .. objfiles)
    end)

target("fs")
    set_kind("object")
    add_includedirs("linux-0.11/include")
    add_cflags("-m32 -g -Wall -fstrength-reduce -fomit-frame-pointer -nostdinc")
    add_files("linux-0.11/fs/*.c")
    after_build(function (target)
        local objfiles = table.concat(target:objectfiles(), " ")
        os.vrun("ld -m elf_i386 -r -o linux-0.11/fs/fs.o " .. objfiles)
    end)

target("mm")
    set_kind("object")
    add_includedirs("linux-0.11/include")
    add_cflags(kernel_cflags)
    add_files("linux-0.11/mm/*.s")
    add_files("linux-0.11/mm/*.c")
    after_build(function (target)
        local objfiles = table.concat(target:objectfiles(), " ")
        os.vrun("ld -m elf_i386 -r -o linux-0.11/mm/mm.o " .. objfiles)
    end)

target("blk_drv")
    set_kind("static")
    set_targetdir("linux-0.11/kernel/blk_drv")
    set_filename("blk_drv.a")
    add_includedirs("linux-0.11/include")
    add_cflags(kernel_cflags)
    add_files("linux-0.11/kernel/blk_drv/*.c")

target("chr_drv/keyboard") -- 请手动`xmake build chr_drv/keyboard`编译
    set_kind("phony")
    set_default(false)
    after_build(function (target)
        -- Compile keyboard.S to keyboard.s
        os.vrun("gcc-3.4 -m32 -g -E -nostdinc -Ilinux-0.11/include -traditional linux-0.11/kernel/chr_drv/keyboard.S -o linux-0.11/kernel/chr_drv/keyboard.s")
    end)

target("chr_drv")
    set_kind("static")
    set_targetdir("linux-0.11/kernel/chr_drv")
    set_filename("chr_drv.a")
    add_includedirs("linux-0.11/include")
    add_cflags(kernel_cflags)
    add_files("linux-0.11/kernel/chr_drv/*.c")
    add_files("linux-0.11/kernel/chr_drv/*.s")

target("math")
    set_kind("static")
    set_targetdir("linux-0.11/kernel/math")
    set_filename("math.a")
    add_cflags(kernel_cflags)
    add_includedirs("linux-0.11/include")
    add_files("linux-0.11/kernel/math/*.c")

target("lib")
    set_kind("static")
    set_targetdir("linux-0.11/lib")
    set_filename("lib.a")
    add_cflags(kernel_cflags)
    add_includedirs("linux-0.11/include")
    add_files("linux-0.11/lib/*.c")

target("system")
    set_kind("phony")
    add_includedirs("linux-0.11/include")
    add_deps("boot/head", "init/main", "kernel", "fs", "mm", "blk_drv", "chr_drv", "math", "lib")
    after_build(function (target) 
        local src_files = {
            "linux-0.11/boot/head.o",
            "linux-0.11/init/main.o",
            "linux-0.11/kernel/kernel.o",
            "linux-0.11/mm/mm.o",
            "linux-0.11/fs/fs.o",
            "linux-0.11/kernel/blk_drv/blk_drv.a",
            "linux-0.11/kernel/chr_drv/chr_drv.a",
            "linux-0.11/kernel/math/math.a",
            "linux-0.11/lib/lib.a"
        }
        os.vrun("ld -m elf_i386 -Ttext 0 -e startup_32 -Ilinux-0.11/include -o linux-0.11/tools/system " .. table.concat(src_files, " "))
        os.vrun("/bin/bash -c \"nm linux-0.11/tools/system | grep -v '\\(compiled\\)\\|\\(\\.o$$\\)\\|\\( [aU] \\)\\|\\(\\.\\.ng$$\\)\\|\\(LASH[RL]DI\\)'| sort > linux-0.11/System.map\"")
    end)
    after_clean(function (target)
        os.rm("linux-0.11/tools/system")
        os.rm("linux-0.11/System.map")
        os.rm("**/*.a")
        os.rm("**/*.o")
    end)

target("build-script")
    set_kind("binary")
    set_toolset("cc", "gcc-11")
    set_targetdir("linux-0.11/tools")
    set_filename("build")
    add_files("linux-0.11/tools/build.c")

target("image")
    set_kind("phony")
    add_deps("boot", "system", "build-script")
    after_build(function (target)
        os.vrun("cp -f linux-0.11/tools/system linux-0.11/tools/system.tmp")
        os.vrun("strip linux-0.11/tools/system.tmp")
        os.vrun("objcopy -O binary -R .note -R .comment linux-0.11/tools/system.tmp linux-0.11/tools/kernel")
        os.vrun("/bin/bash -c \"linux-0.11/tools/build linux-0.11/boot/bootsect linux-0.11/boot/setup linux-0.11/tools/kernel > linux-0.11/Image\"")
    end)
    after_clean(function (target)
        os.rm("linux-0.11/tools/system.tmp")
        os.rm("linux-0.11/tools/kernel")
        os.rm("linux-0.11/Image")
    end)
