#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    // close(0);
    int fd = open("baseCIO.c", O_RDONLY);
    if (fd < 0) {
        perror("open error");
        exit(0);
    }
    printf("fd : %d\n", fd);
    close(fd);

    return 0;
}