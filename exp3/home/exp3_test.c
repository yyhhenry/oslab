#include <stdio.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define HZ 100
#define N 10

void simulate_cpu_io(int last, int cpu_time, int io_time)
{
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0) {
		/* CPU Burst >>>> */
		times(&start_time);
		do {
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			/* Add the system time, since we do not actually have any time-consuming calculations */
			stime = current_time.tms_stime - start_time.tms_stime;
		} while (((utime + stime) / HZ) < cpu_time);
		last -= cpu_time;
		/* <<<< CPU Burst */

		if (last <= 0)
			break;

		/* IO Burst >>>> */
		sleep_time = 0;
		while (sleep_time < io_time) {
			/* Sleep for 1 second simulating some IO operation */
			sleep(1);
			sleep_time++;
		}
		last -= sleep_time;
		/* <<<< IO Burst */
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
	p1 = spwan_task(N, 0, N);
	p2 = spwan_task(N, 2, 8);
	p3 = spwan_task(N, 5, 5);
	p4 = spwan_task(N, 8, 2);
	p5 = spwan_task(N, N, 0);

	wait(NULL);

	printf("Parent PID %d\n", getpid());
	printf("Child PIDs %d %d %d %d %d\n", p1, p2, p3, p4, p5);

	return 0;
}
