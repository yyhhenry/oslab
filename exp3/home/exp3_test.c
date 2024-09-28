#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define HZ 100
#define N 10
#define FOR_RANGE 1000

int sum = 0;

void cpu_consuming_task()
{
	int i;
	int num_sqr_sum;
	int tmp;
	for (i = 0; i < FOR_RANGE; i++) {
		tmp = i;
		num_sqr_sum = 0;
		while (tmp) {
			num_sqr_sum += (tmp % 10) * (tmp % 10);
			tmp /= 10;
		}
		if (num_sqr_sum == i)
			sum++;
	}
}

void io_consuming_task()
{
	sleep(2);
}

void simulate_cpu_io(int last, int cpu_time, int io_time)
{
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0) {
		times(&start_time);
		do {
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
			cpu_consuming_task();
		} while (((utime + stime) / HZ) < cpu_time);
		last -= ((utime + stime) / HZ);

		if (last <= 0)
			break;

		times(&start_time);
		do {
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
			io_consuming_task();
		} while (((utime + stime) / HZ) < io_time);
		last -= io_time;
	}
}

pid_t spwan_task(int last, int cpu_time, int io_time)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		printf("PID %d is forked from PID %d\n", getpid(), getppid());
		simulate_cpu_io(last, cpu_time, io_time);
		exit(0);
	}
	return pid;
}

int main(int argc, char *argv[])
{
	pid_t p1, p2, p3, p4, p5;
	p1 = spwan_task(2 * N, 0, N);
	p2 = spwan_task(2 * N, 2, 8);
	p3 = spwan_task(2 * N, 5, 5);
	p4 = spwan_task(2 * N, 8, 2);
	p5 = spwan_task(2 * N, N, 0);

	wait(NULL);

	printf("Parent PID %d\n", getpid());
	printf("Child PIDs %d %d %d %d %d\n", p1, p2, p3, p4, p5);

	return 0;
}
