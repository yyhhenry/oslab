#define __LIBRARY__
#include <asm/segment.h>
#include <asm/system.h>
#include <errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <string.h>
#include <unistd.h>
#ifdef __INTELLISENSE__
#include <linux/sys.h>
#endif

#define NR_SEMS 30
#define MAX_SEM_NAME 20
#define MAX_SEM_WAIT 10
struct Semaphore {
	char name[MAX_SEM_NAME + 1];
	/* Reference count */
	int ref_cnt;
	int value;
	struct task_struct *sem_wait[MAX_SEM_WAIT];
} semaphore_set[NR_SEMS];

int get_fs_str_eq(const char *user_space_str, const char *sys_space_str)
{
	char c;
	int i;
	for (i = 0; i < MAX_SEM_NAME; i++) {
		c = get_fs_byte(user_space_str + i);
		if (c != sys_space_str[i]) {
			return 0;
		}
		if (c == '\0') {
			break;
		}
	}
	return 1;
}

int get_fs_str_cpy(char *sys_space_str, const char *user_space_str)
{
	char c;
	int i;
	for (i = 0; i < MAX_SEM_NAME; i++) {
		c = get_fs_byte(user_space_str + i);
		sys_space_str[i] = c;
		if (c == '\0') {
			break;
		}
	}
	return i;
}

int sys_sem_open(const char *name, int value)
{
	int i, j, result;
	struct Semaphore *sem;
	char c;
	cli();
	result = -1;
	for (i = 0; i < NR_SEMS && result == -1; i++) {
		sem = &semaphore_set[i];
		if (sem->ref_cnt && get_fs_str_eq(name, sem->name)) {
			sem->ref_cnt++;
			result = i;
		}
	}
	for (i = 0; i < NR_SEMS && result == -1; i++) {
		sem = &semaphore_set[i];
		if (!sem->ref_cnt) {
			sem->ref_cnt = 1;
			get_fs_str_cpy(sem->name, name);
			sem->value = value;
			for (j = 0; j < MAX_SEM_WAIT; j++) {
				sem->sem_wait[j] = NULL;
			}
			result = i;
		}
	}
	sti();
	return result;
}

int sys_sem_wait(int sem)
{
	int tmp;
	struct Semaphore *s;
	cli();
	s = &semaphore_set[sem];
	tmp = s->value;
	s->value--;
	if (-s->value > MAX_SEM_WAIT) {
		s->value++;
		sti();
		return -EAGAIN;
	}
	if (s->value < 0) {
		sleep_on(&s->sem_wait[-tmp]);
	}
	sti();
	return 0;
}

int sys_sem_signal(int sem)
{
	struct Semaphore *s;
	cli();
	s = &semaphore_set[sem];
	s->value++;
	if (s->value <= 0) {
		wake_up(&s->sem_wait[-s->value]);
		s->sem_wait[-s->value] = NULL;
	}
	sti();
	return 0;
}

int sys_sem_close(int sem)
{
	struct Semaphore *s;
	cli();
	s = &semaphore_set[sem];
	if (s->ref_cnt <= 0) {
		sti();
		return -EINVAL;
	}
	s->ref_cnt--;
	sti();
	return 0;
}

#define PC_BUFFER_MAX 100
struct PCBuffer {
	int max_size;
	int push_ptr;
	int pop_ptr;
	/* One extra element for the buffer */
	int buffer[PC_BUFFER_MAX + 1];
} pc_buffer;

int next_ptr(int ptr)
{
	return (ptr + 1) % (pc_buffer.max_size + 1);
}

int sys_buf_init(int max_size)
{
	cli();
	if (max_size <= 0 || max_size > PC_BUFFER_MAX) {
		sti();
		return -EINVAL;
	}
	pc_buffer.max_size = max_size;
	pc_buffer.push_ptr = 0;
	pc_buffer.pop_ptr = 0;
	sti();
	return 0;
}

int sys_buf_push(int value)
{
	cli();
	if (value < 0) {
		sti();
		return -EINVAL;
	}
	if (next_ptr(pc_buffer.push_ptr) == pc_buffer.pop_ptr) {
		sti();
		return -EAGAIN;
	}
	pc_buffer.buffer[pc_buffer.push_ptr] = value;
	pc_buffer.push_ptr = next_ptr(pc_buffer.push_ptr);
	sti();
	return 0;
}

int sys_buf_pop()
{
	int result;
	cli();
	if (pc_buffer.pop_ptr == pc_buffer.push_ptr) {
		sti();
		return -EAGAIN;
	}
	result = pc_buffer.buffer[pc_buffer.pop_ptr];
	pc_buffer.pop_ptr = next_ptr(pc_buffer.pop_ptr);
	sti();
	return result;
}
