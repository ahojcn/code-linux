// UDP 回显服务器

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    // 创建 socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }
    // 绑定 IP 和 port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(8080);
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind");
        return 1;
    }
    printf("server start!\n");

    // 循环处理收到的请求、返回响应
    while (1) {
        // 读取请求
        char buf[1024] = {0};
        struct sockaddr_in client_addr;  // 客户端 ip 和 port
        socklen_t len = sizeof(client_addr);

        ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&client_addr, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;  // 当一次读取失败，服务器不退出
        }
        buf[n] = '\0';

        // 处理请求
        // 因为是回显服务器，原封不动返回给客户端

        // 返回响应
        n = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, len);
        if (n < 0) {
            perror("sendto");
            continue;
        }

        printf("[%s:%d] req: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
    }

    close(fd);
    return 0;
}
