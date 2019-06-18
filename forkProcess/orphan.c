/*
    父进程退出后子进程被 1 号进程 init 领养
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    pid_t ret = fork();
    if (ret == -1) {
        perror("fork");
        exit(-1);
    } else if (ret == 0) {
        // 子进程
        printf("1. i' am child, my father is: %d\n", getppid()); 
        sleep(5);
        printf("2. i' am child, my father is: %d\n", getppid());
    } else {
        // 父进程
        sleep(1);
        printf("i'm going to die.\n");
    }

    return 0;
}