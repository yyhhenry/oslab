#define __LIBRARY__
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

_syscall2(int, sem_open, const char *, name, int, value);
_syscall1(int, sem_wait, int, sem);
_syscall1(int, sem_signal, int, sem);
_syscall1(int, sem_close, int, sem);
_syscall1(int, buf_init, int, max_size);
_syscall1(int, buf_push, int, value);
_syscall0(int, buf_pop);

#undef __LIBRARY__

int sem_product_block;
int sem_empty_block;
int sem_log_file;
char log_buf[1024];

void log_message(const char *message, ...)
{
	int fd;
	va_list args;
	sem_wait(sem_log_file);
	fd = open("sem.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) {
		perror("open");
		exit(-1);
	}

	va_start(args, message);
	vsprintf(log_buf, message, args);
	va_end(args);

	write(fd, log_buf, strlen(log_buf));

	sem_signal(sem_log_file);

	close(fd);
}

int spawn_producer(int count, int idx)
{
	int pid, i;

	if (!(pid = fork())) {
		log_message("Producer %d starts with PID %d\n", idx, getpid());
		for (i = 0; i < count; i++) {
			sem_wait(sem_empty_block);
			buf_push(idx * 1000 + i);
			log_message("Producer %d pushes %d\n", idx, i);
			sem_signal(sem_product_block);
		}
		log_message("Producer %d exits\n", idx);
		exit(0);
	}
	return pid;
}

int spawn_consumer(int count, int idx)
{
	int pid, i, value;

	if (!(pid = fork())) {
		log_message("Consumer %d starts with PID %d\n", idx, getpid());

		for (i = 0; i < count; i++) {
			sem_wait(sem_product_block);
			value = buf_pop();
			log_message("Consumer %d gets %d from Producer %d\n", idx, value % 1000, value / 1000);
			sem_signal(sem_empty_block);
		}
		log_message("Consumer %d exits\n", idx);
		exit(0);
	}
	return pid;
}

int main()
{
	int num_producer, num_consumer;
	int each_producer_count, each_consumer_count;
	int buf_size, i;

	num_producer = 4;
	num_consumer = 3;
	each_producer_count = 3;
	each_consumer_count = 4;
	buf_size = 4;

	sem_product_block = sem_open("product_block", 0);
	sem_empty_block = sem_open("empty_block", buf_size);
	sem_log_file = sem_open("log_file", 1);
	printf("sem_product_block = %d, sem_empty_block = %d\n", sem_product_block, sem_empty_block);

	buf_init(buf_size);

	for (i = 0; i < num_producer; i++) {
		spawn_producer(each_producer_count, i);
	}
	for (i = 0; i < num_consumer; i++) {
		spawn_consumer(each_consumer_count, i);
	}

	wait(NULL);

	sem_close(sem_product_block);
	sem_close(sem_empty_block);
	sem_close(sem_log_file);

	return 0;
}
