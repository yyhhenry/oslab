/**
 * linux/lib/whoami.c
 *
 * (C) 2024 Henry Yan
 */

#define __LIBRARY__
#include <unistd.h>

_syscall2(int, whoami, char *, name, int, size);

_syscall1(int, iam, const char *, name);
