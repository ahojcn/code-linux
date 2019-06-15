#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    pid_t ret = fork();
    if (ret == -1) {
        perror("fork error");
    } else if (ret == 0) {
        // 子进程
        execlp("ls", "ls", "-al", NULL); // 注意第二个参数对应到main函数的参数就是argv[0]，传给ls其实他并没有使用这个参数
    } else {
        // 父进程
        sleep(1);
        printf("father process\n");
    }

    return 0;
}