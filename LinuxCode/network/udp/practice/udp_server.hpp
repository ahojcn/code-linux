#include <cassert>
#include <functional>
#include "udp_socket.hpp"

// 处理函数
// 无返回值
// req：请求
// resp：响应
typedef std::function<void (const std::string& req, std::string* resp)> Handler;

// UDP 通用服务器
class UdpServer {
public:
    UdpServer() {
        if (!m_sock.Create()) {
            perror("UdpServer::UdpServer()");
        }
    }
    ~UdpServer() {
        m_sock.Close();
    }

    bool Start(const std::string& ip, const std::uint16_t& port, Handler handler) {
        bool ret = m_sock.Bind(ip, port);
        if (ret == false) {
            return false;
        }
        while (true) {
            // 接收请求
            std::string req;
            std::string peer_ip;
            std::uint16_t peer_port;
            m_sock.RecvFrom(&req, &peer_ip, &peer_port);

            // 调用处理请求函数
            std::string resp;
            handler(req, &resp);

            // 返回响应
            m_sock.SendTo(resp, peer_ip, peer_port);
        }
    }

private:
    UdpSocket m_sock;
};
