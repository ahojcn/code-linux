// echo_client.c
// UDP 回显客户端

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    // 创建 socket
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // 配置要连接服务器的 ip 和 port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    while (1) {
        // 获取输入
        printf("input> ");
        char input[1024] = {0};
        scanf("%s", &input);
        
        // 将输入发送给服务器
        sendto(fd, input, sizeof(input), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        printf("req: %s\n", input);
        
        // 接收响应
        char resp[1024] = {0};
        recvfrom(fd, resp, sizeof(resp), 0, NULL, NULL);  // NULL, NULL 表示不关注对方的地址
        printf("resp: %s\n", resp);
    }

    close(fd);

    return 0;
}
