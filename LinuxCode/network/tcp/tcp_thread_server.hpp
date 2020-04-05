// 多线程版本服务器

#pragma once

#include <functional>
#include <thread>
#include "tcp_socket.hpp"

#define CHECK_RET(exp) \
    if (!(exp))        \
    {                  \
        return false;  \
    }

typedef std::function<void(const std::string &, std::string *resp)> Handler;
// typedef void(*Handler)(const std::string& req, std::string* reqp);

class TcpThreadServer
{
public:
    TcpThreadServer()
    {
    }
    ~TcpThreadServer()
    {
        listen_sock_.Close();
    }

    bool
    Start(const std::string &ip, const std::uint16_t port, Handler handler)
    {
        // 1. 创建 socket
        CHECK_RET(listen_sock_.Socket());
        // 2. 绑定 ip 端口
        CHECK_RET(listen_sock_.Bind(ip, port));
        // 3. 监听
        CHECK_RET(listen_sock_.Listen());
        // 4. 进入主循环
        while (true)
        {
            // 5. 调用 accept
            TcpSocket cli_sock;
            std::string peer_ip;
            std::uint16_t peer_port;
            bool ret = listen_sock_.Accept(&cli_sock, &peer_ip, &peer_port);
            if (!ret)
            {
                continue;
            }
            printf("[%s:%d] 连接\n", peer_ip.c_str(), peer_port);
            // 6. 创建线程处理客户端逻辑
            ProcessConnect(cli_sock, peer_ip, peer_port, handler);
        }
    }

private:
    TcpSocket listen_sock_;

    void
    ProcessConnect(const TcpSocket &cli_sock, const std::string &ip, const std::uint16_t port, Handler handler)
    {
        // 1. 创建线程
        // 2. 主线程返回
        // 3. 新线程循环处理客户端操作
        //    a. 读取请求  b. 计算响应  c. 把响应写回客户端
        std::thread t(ThreadEntry, cli_sock, ip, port, handler);
        t.detach();
    }

    static void
    ThreadEntry(const TcpSocket& cli_sock, const std::string &ip, const std::uint16_t port, Handler handler)
    {
        // a. 读取请求  b. 计算响应  c. 把响应写回客户端
        while (true)
        {
            std::string req;
            int ret = cli_sock.Recv(&req);
            if (ret < 0) {
                continue;
            }
            if (ret == 0) {
                printf("[%s:%d] 断开连接\n", ip.c_str(), port);
                cli_sock.Close();
                break;
            }
            printf("[%s:%d] req:%s\n", ip.c_str(), port, req.c_str());
            std::string resp;
            handler(req, &resp);
            cli_sock.Send(resp);
        }
    }
};
