// 通用 tcp 服务器
#pragma once

#include <functional>
#include "tcp_socket.hpp"

#define CHECK_RET(exp) if (!(exp)) {\
    return false;\
}

typedef std::function<void(const std::string&, std::string*)> Handler;  // C++ 11，可以接收函数指针、仿函数、Lambda表达式

class TcpServer {
public:
    TcpServer() {}

    // 存在问题，第一个客户端连接后 accept 进入一个循环
    // 当第一个连接没有结束的时候一直在循环内处理第一个客户端的请求
    // 后续的客户端连接过来的时候在内核的连接队列中排队等待中，第一个连接的客户端退出后才会得到 accpet
    // 多进程、多线程（当连接后创建一个进程/线程对客户端进行服务）
    bool Start(const std::string& ip, uint16_t port, Handler handler) {
        // 1. 创建 socket
        CHECK_RET(listen_sock_.Socket());
        // 2. 绑定端口号
        CHECK_RET(listen_sock_.Bind(ip, port));
        // 3. 进行监听
        CHECK_RET(listen_sock_.Listen());

        printf("Server Start OK...\n");
        // 4. 进入主循环
        while (true) {
            // 5. 通过 accept 获取到一个连接
            TcpSocket cli_sock;
            std::string ip;
            uint16_t port;
            bool ret = listen_sock_.Accept(&cli_sock, &ip, &port);
            if (!ret) {
                continue;
            }
            printf("[%s:%d] 连接\n", ip.c_str(), port);
            // 6. 和客户端进行具体的沟通, 一次连接中和客户端进行多次交互
            while (true) {
                // 读取请求
                std::string req;
                int r = cli_sock.Recv(&req);
                if (r < 0) {
                    continue;
                }
                if (r == 0) {
                    // 对端关闭了 socket
                    cli_sock.Close();
                    printf("[%s:%d]对方关闭了连接\n", ip.c_str(), port);
                    break;
                }
                printf("[%s:%d]客户端发送了：%s\n", ip.c_str(), port, req.c_str());
                // 根据请求计算响应
                std::string resp;
                handler(req, &resp);
                // 响应写回客户端
                cli_sock.Send(resp);
            }
        }
    }
private:
    TcpSocket listen_sock_;
};
