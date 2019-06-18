/*
    调用 wait 回收子进程资源
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
        sleep(100);
        exit(101);
    } else {
        // 父进程
        int status = 0;
        pid_t w_ret = wait(&status);
        if (w_ret == -1) {
            perror("wait error");
            exit(-2);
        }
        if (WIFEXITED(status)) {
            printf("child exit with %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("child exit by sig %d\n", WTERMSIG(status));
        }

        while (1) {
            sleep(2);
        }
    }

    return 0;
}