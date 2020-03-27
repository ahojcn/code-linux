// 练习 udp 服务器

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  // 1. 创建一个 socket
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("socket");
    return -1;
  }

  // 2. 绑定一个 ip 和 port
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  addr.sin_port = htons(8080);
  int ret = bind(sock_fd, (sockaddr*)&addr, sizeof(addr));
  if (ret < 0) {
    perror("bind");
    return -1;
  }

  printf("服务器已经启动！\n");

  // 3. 处理服务器收到的请求
  while (true) {
    // 接收请求数据
    char buf[1024] = {0};
    sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t n = recvfrom(sock_fd, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
    if (n < 0) {
      perror("recvfrom");
      continue;
    }
    buf[n] = '\0';
    
    printf("req: [%s:%d](%d) %s\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), n, buf);

    // 处理请求数据

    // 返回响应
    n = sendto(sock_fd, buf, strlen(buf), 0, (sockaddr*)&peer, len);
    if (n < 0) {
      perror("sendto");
      continue;
    }
  }

  return 0;
}
