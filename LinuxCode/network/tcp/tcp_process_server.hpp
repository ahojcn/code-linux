#pragma once

#include <functional>
#include <signal.h>
#include "tcp_socket.hpp"

#define CHECK_RET(exp) \
    if (!(exp))        \
    {                  \
        return false;  \
    }

typedef std::function<void(const std::string &, std::string *resp)> Handler;

class TcpProcessServer
{
public:
    TcpProcessServer() {}
    ~TcpProcessServer()
    {
        listen_sock_.Close();
    }

    bool Start(const std::string &ip, const std::uint16_t port, Handler handler)
    {
        signal(SIGCLD, SIG_IGN); // 回收子进程

        // 1. 创建 socket
        CHECK_RET(listen_sock_.Socket());
        // 2. 绑定 ip 端口
        CHECK_RET(listen_sock_.Bind(ip, port));
        // 3. 监听
        CHECK_RET(listen_sock_.Listen());
        // 4. 进入主循环
        while (true)
        {
            // 调用 accept
            TcpSocket cli_socket;
            std::string peer_ip;
            std::uint16_t peer_port;
            bool ret = listen_sock_.Accept(&cli_socket, &peer_ip, &peer_port);
            if (!ret)
            {
                continue;
            }
            printf("[%s:%d] 连接\n", peer_ip.c_str(), peer_port);
            // 创建子进程，让子进程处理客户端请求，父进程继续调用 accept
            ProcessConnect(cli_socket, peer_ip, peer_port, handler);
        }
    }

private:
    TcpSocket listen_sock_;

    void ProcessConnect(const TcpSocket &cli_socket, const std::string &ip, const std::uint16_t port, Handler handler)
    {
        // 1. 创建子进程
        // 2. 父进程直接结束这个函数
        // 3. 子进程循环做下面的事情
        //    a. 读取客户端请求
        //    b. 计算响应
        //    c. 把响应写回给客户端
        pid_t ret = fork();
        if (ret > 0)
        {
            cli_socket.Close();  // 注意父进程也需要关闭这个 socket 否则文件描述符泄露
            return;
        }
        while (true)
        {
            std::string req;
            int r = cli_socket.Recv(&req);
            if (r < 0)
            {
                continue;
            }
            if (r == 0)
            {
                printf("[%s:%d] 客户端断开连接\n", ip.c_str(), port);
                break;
                // exit(0);
            }
            printf("[%s:%d] req: %s\n", ip.c_str(), port, req.c_str());
            std::string resp;
            handler(req, &resp);
            cli_socket.Send(resp);
        }
        // 子进程收尾工作
        cli_socket.Close();
        exit(0);
    }
};
