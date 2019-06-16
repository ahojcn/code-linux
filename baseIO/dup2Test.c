#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    int fd = open("log", O_CREAT | O_RDWR);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }
    dup2(fd, 1);
    printf("hahahahaha\n");

    close(fd);

    return 0;
}