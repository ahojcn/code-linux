#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 写文件
void writeFile() {
    int fd = open("test", O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }
    
    const char* msg = "hello sys io\n";
    for (size_t i = 0; i < 5; i++) {
        // msg:缓冲区首地址， len: 本次读取，期望写入多少个字节的数据。 
        // 返回值:实际写了多少字节数据
        ssize_t real_len = write(fd, msg, strlen(msg));
        printf("real len = %lu\n", real_len);
    }

    close(fd);
}

// 读文件
void readFile() {
    int fd = open("test", O_RDONLY);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }

    const char* msg = "hello sys io\n";
    char buf[1024] = {0};
    while (1) {
        ssize_t s = read(fd, buf, strlen(msg));
        if (s > 0) {
            printf("buf = %s", buf);
        } else {
            break;
        }
    }
}

int main(void) {
    writeFile();
    
    readFile();

    return 0;
}