#include <asm/segment.h>
#include <fcntl.h>
#include <kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int get_exec_name(struct task_struct *p, char *name)
{
	int i;
	char *basename = NULL;
	struct m_inode *inode = NULL;
	struct buffer_head *bh = NULL;
	struct dir_entry *de = NULL;

	if (p->pid < 0) {
		return -1;
	}

	inode = p->executable;
	if (inode == NULL) {
		return -1;
	}

	bh = bread(inode->i_dev, inode->i_zone[0]);
	if (bh == NULL) {
		return -1;
	}

	de = (struct dir_entry *)bh->b_data;
	for (i = 0; i < inode->i_size / sizeof(struct dir_entry); i++, de++) {
		if (de->inode == inode->i_num) {
			basename = de->name;
			break;
		}
	}

	if (basename == NULL) {
		return -1;
	}

	strcpy(name, basename);
	return 0;
}

char *state_str(struct task_struct *p)
{
	switch (p->state) {
	case TASK_RUNNING:
		if (current->pid == p->pid) {
			return "running";
		} else {
			return "ready";
		}
	case TASK_INTERRUPTIBLE:
	case TASK_UNINTERRUPTIBLE:
		return "waiting";
	case TASK_ZOMBIE:
	case TASK_STOPPED:
		return "stopped";
	default:
		return "-";
	}
}

char psinfo_buf[1024];
char name_buf[256];

int psinfo_read(int dev, char *buf, int count, off_t *f_pos)
{
	struct task_struct *p;
	int i, n;

	n = 0;
	n += sprintf(psinfo_buf + n, "pid\texec\tstate\tuid\tfather\tstart\n");
	for (i = 0; i < NR_TASKS; i++) {
		p = task[i];
		if (p == NULL) {
			continue;
		}
		if (get_exec_name(p, name_buf)) {
			sprintf(name_buf, "-");
		}
		n += sprintf(psinfo_buf + n, "%d\t%s\t%s\t%d\t%d\t%d\n", (int)p->pid, name_buf, state_str(p), (int)p->uid, (int)p->father, (int)p->start_time);
	}

	for (i = 0; i < count && i + (*f_pos) < n; i++) {
		put_fs_byte(psinfo_buf[i + (*f_pos)], buf + i + (*f_pos));
	}
	(*f_pos) = (*f_pos) + i;
	return i;
}
