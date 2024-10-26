#define __LIBRARY__
#include <asm/system.h>
#include <linux/mm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

_syscall3(int, share_memory_with, int, key, int, size, unsigned long *, p_addr);

char *messages[] = {
	"Hello, World!",
	"Goodbye, Earth!",
	"Now, we are in the space!",
	"Let's go to the Mars!",
	"",
};

int main()
{
	int i, j;
	char *shared;
	int length, base;
	share_memory_with(8080, PAGE_SIZE, (unsigned long *)&shared);
	printf("Producer is ready. (shared: %p)\n", shared);
	shared[0] = 0;

	while (messages[length][0]) {
		length++;
	}

	for (i = 0; i < length; i++) {
		while (shared[0]) {
			sleep(1);
		}
		printf("[Background] Producer: Sending %d/%d...\n", i + 1, length);
		fflush(stdout);
		for (j = 0; messages[i][j]; j++) {
			shared[j + 1] = messages[i][j];
		}
		shared[j + 1] = 0;
		shared[0] = 1;
	}
	while (shared[0]) {
		sleep(1);
	}
	shared[1] = 0;
	shared[0] = 1;
	printf("Sending `done` signal...\n");
	return 0;
}
