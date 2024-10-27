# 调试内核并手动映射地址

## 准备

添加代码并添加标记 `exp_code`, `exp_data` 并编译（见[head.s](./exp_head.s)）。
从 `System.map` 找到标记的地址 `exp_code: 0x54a5`, `exp_data: 0x64c0`。
依次执行 `b 0x54a5`, `c`, `n`, `n`，然后 `reg` 查看 `eax` 的值为 `0xbad0beef`。

## 查询 GDT 表

`info gdt 0x00 0x02` 直接查看 GDT 表，找到 `0x02` 对应的数据段，起始地址为 `0x00000000`。

相当于 `sreg` 找到 `gdtr:base=0x00005cc0`，然后 `xp /6w 0x5cc0` 查看 GDT 表前 3 项。

```txt
Global Descriptor Table (base=0x00005cc0, limit=2047):
GDT[0x00]=??? descriptor hi=0x00000000, lo=0x00000000
GDT[0x01]=Code segment, laddr=00000000, limit=00fff * 4Kbytes, Execute/Read, 32-bit
GDT[0x02]=Data segment, laddr=00000000, limit=00fff * 4Kbytes, Read/Write, Accessed
```

## 查询页表

`info tab` 查看页表，`0x00000000-0x00ffffff -> 0x00000000-0x00ffffff`，即物理地址恰好等于逻辑地址。

`xp /w 0x64c0` 查看物理地址的值，为 `0xbad0beef`。

相当于

- 分解 `0x000064c0` 得到10位页目录号 `0x000`，10位页表号 `0x006`，12位页内偏移 `0x4c0`。
- `xp /w 0x0` 查看页目录项，得到 `0x00001027`。
- `xp /w 0x1000+6*4` 查看`0x00001018`处的页表项，得到 `0x00006027`。
- `xp /w 0x6000+0x4c0` 查看`0x000064c0`处的数据，得到 `0xbad0beef`。
