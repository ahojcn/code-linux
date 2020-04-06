#pragma once

#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

class TcpSocket{
public:
    TcpSocket() : fd_(-1) {}

    TcpSocket(int fd) : fd_(fd) {}

    bool Socket() {
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) {
            perror("socket");
            return false;
        }
        return true;
    }

    bool Bind(const std::string& ip, uint16_t port) {
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

    bool Listen() {
        // 进入监听状态
        int ret = listen(fd_, 10);
        if (ret < 0) {
            perror("listen");
            return false;
        }
        return true;
    }

    bool Accept(TcpSocket* peer, std::string* ip = NULL, uint16_t* port = NULL) {
        // 从连接队列中取一个连接到用户代码中
        // 如果队列中没有连接，阻塞(默认行为)
        sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        // 返回值也是一个 socket
        int client_sock = accept(fd_, (sockaddr*)&peer_addr, &len);
        if (client_sock < 0) {
            perror("accept");
            return false;
        }

        peer->fd_ = client_sock;
        if (ip != NULL) {
            *ip = inet_ntoa(peer_addr.sin_addr);
        }
        if (port != NULL) {
            *port = ntohs(peer_addr.sin_port);
        }
        return true;
    }

    int Recv(std::string* msg) const {
        msg->clear();
        char buf[1024 * 10] = {0};
        ssize_t n = recv(fd_, buf, sizeof(buf) - 1, 0);
        // recv 的返回值：如果读取成功，返回结果为读到的字节数
        // 读取失败，返回 -1
        // 对端关闭 socket 返回结果为 0
        if (n < 0) {
            perror("recv");
            return -1;
        } else if (n == 0) {
            return 0;
        }
        msg->assign(buf);
        return 1;
    }

    bool Send(const std::string& msg) const {
        ssize_t n = send(fd_, msg.c_str(), msg.size(), 0);
        if (n < 0) {
            perror("send");
            return false;
        }
        return true;
    }

    // 给客户端使用
    bool Connect(const std::string& ip, uint16_t port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        int ret = connect(fd_, (sockaddr*)&addr, sizeof(addr));
        if (ret < 0) {
            perror("connect");
            return false;
        }
        return true;
    }

    bool Close() const {
        if (fd_ != -1) {
            close(fd_);
        }
        return true;
    }

    // et 边缘触发
    bool SetNoBlock() {
        // 获取 fd 的相关标志位，也是一个位图的方式，保存在 fl 中
        int fl = fcntl(fd_, F_GETFL);
        if (fl < 0) {
            perror("fcntl F_GETFL");
            return false;
        }
        int ret = fcntl(fd_, F_SETFL, fl | O_NONBLOCK);
        if (ret < 0) {
            perror("fcntl F_SETFL");
            return false;
        }
        return true;
    }
    // 非阻塞读
    int RecvNoBlock(std::string* msg) const {
        msg->clear();
        // 注意，下面的流程没有考虑黏包问题，是一个近似的写法，不够严谨
        // 如果 tcp 接收缓冲区为空，此时会读不到数据，但是 recv 会返回，errno 会被设置成 EAGAIN 或 EWOULDBLOCK
        // 如果当前读到数据长度比设定的用户缓冲区长度一样，就持续往下读
        // 如果当前读到的数据长度比缓冲区短了，就认为暂时读完了
        char tmp[1024 * 10] = {0};
        while (true) {
            ssize_t n = recv(fd_, tmp, 1024 * 10 - 1, 0);
            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 缓冲区无数据，继续尝试读
                    continue;
                }
                return -1;  // 其他原因直接退出函数
            }
            if (n == 0) {
                // 对端关闭
                return 0;
            }
            tmp[n] = '\0';
            // 把这个数据追加到 msg 中
            msg->append(tmp); // msg += tmp

            if (n < (ssize_t)sizeof(tmp) - 1) {  // 这个条件是近似的，不严谨的
                // 这次读取没有填充完整个缓冲区，当前缓冲区暂时没数据了
                break;
            }
        }

        return msg->size();
    }
    // 非阻塞写
    bool SendNoBlock(const std::string& msg) const {
        // 当实际发送数据长度和 msg 长度一样的时候就发送完了，这个是准确的

        // 这样写有风险，由于当前是非阻塞 IO，如果发送缓冲区已经慢了，函数调用就返回，可能会导致发送失败的情况
        send(fd_, msg.c_str(), msg.size(), 0);
        // 更严谨的情况，每次小块小块的发送，每次发送完看看自己发了多少，最终累加发送成功的数据达到 msg 长度的时候，才算完成  TODO
    }

    int GetFd() const { return fd_; }
private:
    int fd_;
};
