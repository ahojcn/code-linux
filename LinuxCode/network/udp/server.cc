// udp 版本服务器

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    // 1. 创建一个 socket
    // AF_INET 表示使用 IPv4 协议
    // SOCK_DGRAM 表示使用 UDP 协议
    // 第三个参数不用填 0
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // 2. 把当前的 socket 绑定一个 IP 和 端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");  // ip 地址也是一个整数需要转成网络字节序，这个函数帮忙转了
    addr.sin_port = htons(8080);  // 端口号必须转成网络字节序
    int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind");
        return 1;
    }

    printf("服务器启动完成！\n");

    // 3. 处理服务器收到的请求
    while (true) {
        // a. 读取客户端请求
        char buf[1024] = {0};
        sockaddr_in peer;
        socklen_t len = sizeof(peer);  // 输出输出参数
        ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buf[n] = '\0';

        // b. 处理请求
        // ...

        // c. 返回给客户端
        n = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&peer, len);
        if (n < 0) {
            perror("sendto");
            continue;
        }

        printf("[%s:%d] buf: %s\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), buf);
    }
    
    close(sock);
    return 0;
}
