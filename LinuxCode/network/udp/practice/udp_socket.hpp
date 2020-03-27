#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UdpSocket {
public:
    UdpSocket() : fd_(-1) {}
    // 创建一个 socket（客户端、服务器）
    bool Socket() {
        fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd_ < 0) {
            return false;
        }
        return true;
    }
    // 绑定 ip 和 端口号（服务器）
    bool Bind(const std::string& ip, const std::uint16_t port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        int ret = bind(fd_, (sockaddr*)&addr, sizeof(addr));
        if (ret < 0) {
            perror("bind");
            return false;
        }

        return true;
    }
    // 关闭 socket 文件描述符（客户端、服务器）
    bool Close() {
        if (fd_ != -1) {
            close(fd_);
        }
        return true;
    }
    // 接收数据（客户端、服务器）
    bool RecvFrom(std::string* msg, std::string* ip = NULL, std::uint16_t* port = NULL) {
        char buf[1024 * 10] = {0};
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t n = recvfrom(fd_, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
        if (n < 0) {
            perror("recvfrom");
            return false;
        }
        *msg = buf;
        if (ip != NULL) {
            *ip = inet_ntoa(peer.sin_addr);
        }
        if (port != NULL) {
            *port = ntohs(peer.sin_port);
        }

        return true;
    }
    // 发送数据 msg
    bool SendTo(const std::string& msg, const std::string& ip, const std::uint16_t& port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        ssize_t n = sendto(fd_, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
        if (n < 0) {
            perror("sendto");
            return false;
        }
        return true;
    }
private:
    int fd_;
};
