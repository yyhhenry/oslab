# exp2 codes

```cpp
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
        printf("%s\n", temp);
    } else {
        perror("whoami error");
        return -1;
    }
    return 0;
}
```