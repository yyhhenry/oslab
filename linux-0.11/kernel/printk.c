/*
 *  linux/kernel/printk.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * When in kernel-mode, we cannot use printf, as fs is liable to
 * point to 'interesting' things. Make a printf with fs-saving, and
 * all is well.
 */
#include <stdarg.h>
#include <stddef.h>

#include <linux/kernel.h>
#include <linux/sched.h>

static char buf[1024];
char log_f_buf[1024];

extern int vsprintf(char * buf, const char * fmt, va_list args);

int printk(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $buf\n\t"
		"pushl $0\n\t"
		"call tty_write\n\t"
		"addl $8,%%esp\n\t"
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i):"ax","cx","dx");
	return i;
}

int log_f(const char *fmt, ...)
{
	va_list args;
	int i, fd;
	struct file *file;
	struct m_inode *inode;

	va_start(args, fmt);
	i = vsprintf(log_f_buf, fmt, args);
	va_end(args);

	fd = 3;

	if (!(file = task[0]->filp[fd])) {
		if (task[0] && current) {
			printk("file is NULL\n");
		}
		return -1;
	}
	inode = file->f_inode;
	__asm__("push %%fs\n\t"
			"push %%ds\n\t"
			"pop %%fs\n\t"
			"pushl %0\n\t"
			"pushl $log_f_buf\n\t"
			"pushl %1\n\t"
			"pushl %2\n\t"
			"call file_write\n\t"
			"addl $12,%%esp\n\t"
			"popl %0\n\t"
			"pop %%fs" ::"r"(i),
			"r"(file), "r"(inode) : "ax", "cx", "dx");
	return i;
}
