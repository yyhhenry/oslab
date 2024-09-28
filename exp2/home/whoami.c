#define __LIBRARY__
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define N 256
_syscall2(int, whoami, char *, name, unsigned int, size);

int main() {
    int num;
    char temp[N] = {0};

    num = whoami(temp, N);
    if (num >= 0) {
        printf("I am %s\n", temp);
    } else {
        printf("%d\n", num);
        perror("`whoami` error");
        return -1;
    }
    return 0;
}
