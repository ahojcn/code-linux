#include "udp_socket.hpp"

class UdpClient {
public:
    UdpClient(const std::string& ip, const std::uint16_t& port) : m_sv_ip(ip), m_sv_port(port) {
        if (!m_sock.Create()) {
            perror("UdpClient::UdpClent()");
        }
    }
    ~UdpClient() { 
        if (!m_sock.Close()) {
            // 关闭失败，打印错误信息
            perror("UdpClient::~UdpClient()");
        }
    }

    bool GetResp(std::string* resp) {
        return m_sock.RecvFrom(resp);
    }
    bool SetReq(const std::string& req) {
        return m_sock.SendTo(req, m_sv_ip, m_sv_port);
    }

private:
    UdpSocket m_sock;
    std::string m_sv_ip; // 服务端ip
    std::uint16_t m_sv_port;  // 服务器port
};
