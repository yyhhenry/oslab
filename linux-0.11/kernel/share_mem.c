#define __LIBRARY__
#include <asm/segment.h>
#include <asm/system.h>
#include <errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <unistd.h>
#ifdef __INTELLISENSE__
#include <linux/sys.h>
#endif
#define SHARED_MEMORY_MAX 10

#define LOW_MEM 0x100000
#define MAP_NR(addr) (((addr) - LOW_MEM) >> 12)

struct SharedMemoryBlock {
	int key;
	int size;
	unsigned long page;
} shared_memory_blocks[SHARED_MEMORY_MAX];

int sys_share_memory_with(int key, int size, unsigned long *p_addr)
{
	int i;
	unsigned long page;
	unsigned long mem_map_idx;
	unsigned long addr;
	cli();

	if (size > PAGE_SIZE)
		return -EINVAL;
	if (key <= 0)
		return -EINVAL;

	for (i = 0; i < SHARED_MEMORY_MAX; i++) {
		if (shared_memory_blocks[i].key == key) {
			if (shared_memory_blocks[i].size != size)
				return -1;

			page = shared_memory_blocks[i].page;
			addr = current->brk;
			sys_brk(addr + PAGE_SIZE);
			mem_map_idx = MAP_NR(addr);
			put_page_share(page, current->start_code + addr);
			put_fs_long(addr, p_addr);
			sti();
			return 0;
		}
	}
	for (i = 0; i < SHARED_MEMORY_MAX; i++) {
		if (shared_memory_blocks[i].key == 0) {
			shared_memory_blocks[i].key = key;
			shared_memory_blocks[i].size = size;
			page = get_free_page();
			shared_memory_blocks[i].page = page;
			addr = current->brk;
			sys_brk(addr + PAGE_SIZE);
			put_page(page, current->start_code + addr);
			put_fs_long(addr, p_addr);
			sti();
			return 0;
		}
	}
	sti();
	return -1;
}
