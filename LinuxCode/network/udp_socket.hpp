#ifndef __UDP_SOCKET__
#define __UDP_SOCKET__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

class UdpSocket {
public:
	UdpSocket() : m_fd(-1) {}
    ~UdpSocket() {
        Close();
    }

    // 创建一个 UDP socket
    // 创建成功返回 true，失败返回 false
	bool Create() {
        m_fd = socket(AF_INET, SOCK_DGRAM, 0);
        return m_fd == -1 ? false : true;
    }
    // 关闭 socket
    bool Close() {
        return close(m_fd) == -1 ? false : true;
    }
    // 绑定 ip 和 port
    bool Bind(const std::string& ip, const std::uint16_t& port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        return bind(m_fd, (sockaddr*)&addr, sizeof(addr)) == -1 ? false : true;
    }
    // 接收数据
    bool RecvFrom(std::string* msg, std::string* ip=nullptr, std::uint16_t* port=nullptr) {
        char buf[1024] = { 0 };
        sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);
        ssize_t n = recvfrom(m_fd, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &peer_len);
        if (n < 0) {
            return false;
        }
        *msg = buf;
        if (ip != nullptr) {
            *ip = inet_ntoa(peer.sin_addr);
        }
        if (port != nullptr) {
            *port = ntohs(peer.sin_port);
        }
        return true;
    }
    // 发送数据
    bool SendTo(const std::string& msg, const std::string& ip, const std::uint16_t& port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        ssize_t n = sendto(m_fd, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
        return n == -1 ? false : true;
    }

private:
	int m_fd;
};

#endif // __UDP_SOCKET__

