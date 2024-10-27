#define __LIBRARY__
#include <linux/mm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

_syscall3(int, share_memory_with, int, key, int, size, unsigned long *, p_addr);

int main()
{
	int i;
	char *shared;
	share_memory_with(8080, PAGE_SIZE, (unsigned long *)&shared);

	printf("Consumer is ready. (shared: %p)\n", shared);
	fflush(stdout);
	while (1) {
		while (!shared[0]) {
			sleep(1);
		}

		if (!shared[1]) {
			break;
		}

		printf("Consumer: Received message `%s`\n\n", shared + 1);
		fflush(stdout);
		shared[0] = 0;
	}
	printf("Received `done` signal.\n");
	return 0;
}
