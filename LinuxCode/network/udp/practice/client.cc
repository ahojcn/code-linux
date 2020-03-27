// 练习 udp 客户端

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    // 1. 创建一个 socket
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    // 2. 设置要连接的服务器 port 和 ip
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(8080);

    // 3. 发送请求，获取服务器的响应
    while (true) {
        char buf[1024] = {0};
        printf("> ");
        scanf("%s", &buf);
        sendto(sock_fd, buf, strlen(buf), 0, (sockaddr*)&addr, sizeof(addr));

        // 获取响应
        char resp[1024] = {0};
        recvfrom(sock_fd, resp, sizeof(resp), 0, NULL, NULL);
        printf("resp: %s\n", resp);
    }

    return 0;
}
