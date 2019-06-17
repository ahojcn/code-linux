/*
    循环创建子进程
 */
#include <stdio.h>
#include <unistd.h>

int main(void) {
    size_t i = 0;

    for (i = 0; i < 5; i++) {
        pid_t ret = fork();
        if (ret == -1) {
            perror("fork error");
        } else if (ret == 0) {
            // 子进程
            break;
        } else {
            // 父进程
        }
    }

    if (i < 5) {
        printf("I'am %lu child, pid = %d\n", i+1, getpid());
    }

    return 0;
}
