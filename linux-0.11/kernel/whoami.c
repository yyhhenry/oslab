#include <asm/segment.h>
#include <errno.h>

#define MAX_LEN_USERNAME 20

char sys_username[MAX_LEN_USERNAME + 1];

/// @brief Get the current user name
int sys_whoami(char * /* User space */ name, int size) {
    if (size < 1) {
        // At least one byte is required for the '\0' character
        return -EINVAL;
    }
    int i;
    for (i = 0; sys_username[i] && i + 1 < size; i++) {
        put_fs_byte(sys_username[i], name + i);
    }
    put_fs_byte('\0', name + i);
    return 0;
}

/// @brief Set the current user name
int sys_iam(const char * /* User space */ name) {
    int i;
    char c;
    for (i = 0; i < MAX_LEN_USERNAME && (c = get_fs_byte(name + i)); i++) {
        sys_username[i] = c;
    }
    sys_username[i] = '\0';
    return 0;
}
