#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	float sum, current;
	sum = 0.0;

	for (current = 1.0;; current /= 2) {
		sum += current;
	}
	return 0;
}
