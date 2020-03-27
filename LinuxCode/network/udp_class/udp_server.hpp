#pragma once

#include "udp_socket.hpp"
#include <functional>
#include <cassert>

// 通用 UDP 服务器类
// 1. 读取请求
// 2. 根据请求计算响应 （回调函数，函数指针的方法是不太 c++ 的，仿函数）
// 3. 把响应写回客户端
// 1 和 3 是固定套路，2 和业务相关，所以把 2 对应代码提取成回调函数

// typedef void (*Handler)(const std::string& req, std::string* resp);
typedef std::function<void(const std::string&, std::string*)> Handler;  // C++ 11，可以接收函数指针、仿函数、Lambda表达式

class UdpServer {
public:
    UdpServer() {
        assert(sock_.Socket());
    }

    ~UdpServer() {
        sock_.Close();
    }

    bool Start(const std::string& ip, const uint16_t port, Handler handler) {
        // 1. 创建 socket (构造函数已经完成)
        // 2. 绑定 ip 端口
        bool ret = sock_.Bind(ip, port);
        if (ret == false) {
            return false;
        }
        while (true) {
            // 处理每一个请求
            // a 读取请求
            std::string req;
            std::string peer_ip;
            std::uint16_t peer_port;
            sock_.RecvFrom(&req, &peer_ip, &peer_port);
            // b 根据请求计算响应
            std::string resp;
            handler(req, &resp);
            // c 把响应返回给客户端
            sock_.SendTo(resp, peer_ip, peer_port);
        }
    }
private:
    UdpSocket sock_;
};
