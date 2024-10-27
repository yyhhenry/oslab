#define __LIBRARY__
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define N 256
_syscall1(int, iam, char *, name);

int main(int argc, char *argv[]) {
    int num;
    if (argc < 2) {
        fprintf(stderr, "Usage: iam <name>\n");
        return -1;
    }
    num = iam(argv[1]);
    if (num < 0) {
        perror("`iam` error");
        return -1;
    }
    printf("User name set to %s\n", argv[1]);
    return 0;
}
