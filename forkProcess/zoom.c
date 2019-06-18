/*
    子进程退出后 父进程没有回收资源 产生僵尸进程
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    pid_t ret = fork();

    if (ret == -1) {
        perror("fork error");
        exit(-1);
    } else if (ret == 0) {
        // 子进程
        printf("child process : i'm going to die.\n");
    } else {
        // 父进程
        while (1) {
            printf("father process : pid = %d, and my son pid = %d\n", getpid(), ret);
            sleep(2);
        }
    }

    return 0;
}