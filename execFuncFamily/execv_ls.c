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
        char* argv[] = {"ls", "-a", NULL};
        execv("/bin/ls", argv);
    } else {
        // 父进程
        sleep(1);
        printf("father process\n");
    }
    return 0;
}