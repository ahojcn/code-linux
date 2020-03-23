// 客户端

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    // 1. 创建 socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // 2. 客户端一般不需要 bind，没有 bind 操作系统随机分配端口号

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(8080);

    // 3. 直接发送数据即可
    while (1) {
        char buf[1024] = {0};
        printf("input: ");
        scanf("%s", &buf);
        sendto(sock, buf, strlen(buf), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        // 从服务器接收返回结果
        char buf_output[1024] = {0};
        recvfrom(sock, buf_output, sizeof(buf_output), 0, NULL, NULL);  //  NULL, NULL 表示不关注对端地址
        printf("resp: %s\n", buf_output);
    }

    return 0;
}