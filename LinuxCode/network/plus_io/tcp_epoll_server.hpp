#pragma once

#include <sys/epoll.h>
#include <vector>
#include <functional>
#include "tcp_socket.hpp"

// 接口
class Base {
public:
    virtual void Add(const TcpSocket&) = 0;
    virtual void Del(const TcpSocket&) = 0;
    virtual void Wait(std::vector<TcpSocket>* output) = 0;
};

// 对标 Selector 类
class Epoll {
public:
    Epoll() {
        epoll_fd_ = epoll_create(10);
    }
    ~Epoll() {
        close(epoll_fd_);
    }

    void Add(const TcpSocket& sock, bool is_et = false) {
        printf("[Epoll::Add] %d\n", sock.GetFd());
        epoll_event event;
        event.events = EPOLLIN;
        if (is_et) {  // 边缘触发
            event.events |= EPOLLET;
        }

        // 此处 epoll add 插入键值对，fd 在键和值之中都出现了，这件事情迫不得已，这也是 epoll 的小槽点
        event.data.fd = sock.GetFd();
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock.GetFd(), &event);
    }
    void Del(const TcpSocket& sock) {
        printf("[Epoll:Del] %d\n", sock.GetFd());
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock.GetFd(), NULL);
    }
    void Wait(std::vector<TcpSocket>* output) {
        output->clear();
        // 等待文件描述符就绪
        epoll_event events[100];
        // -1 表示阻塞等待
        // 返回之后就有若干个文件描述符就绪，保存在 events 数组之中，返回值代表数组有多少有效元素
        int nfds = epoll_wait(epoll_fd_, events, 100, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            return;
        }
        // 依次处理每个就绪的文件描述符，注意这里没有取得 epoll 对应的 key，所以上面把 fd 放到值里面
        for (int i = 0;i < nfds; ++i) {
            int sock = events[i].data.fd;
            output->push_back(TcpSocket(sock));
        }
    }

private:
    int epoll_fd_;  // 通过这个 fd 就可以找到内核中的对象进行文件描述符管理
};

// 实现一个 epoll 版本的 TCP server
typedef std::function<void (const std::string&, std::string*)> Handler;

#define CHECK_RET(exp) \
    if (!exp) { \
        return false; \
    } \

class TcpEpollServer {
public:
    bool Start(const std::string& ip, std::uint16_t port, Handler handler) {
        // 1. 先创建 socket
        // 2. 绑定端口号
        // 3. 监听 socket
        TcpSocket listen_sock;
        CHECK_RET(listen_sock.Socket());
        CHECK_RET(listen_sock.Bind(ip, port));
        CHECK_RET(listen_sock.Listen());
        // 4. 创建 Epoll 对象，并把 listen_sock 使用 Epoll 管理
        Epoll epoll;
        epoll.Add(listen_sock);
        // 5. 进入主循环
        while (true) {
            // 6. 使用 Epoll::Wait 等待文件描述符就绪
            std::vector<TcpSocket> output;
            epoll.Wait(&output);
            // 7. 循环处理每个就绪的文件描述符，也是分为两种情况
            //    a) 如果是 listen_sock 就调用 Accept
            //    b) 如果不是调用 Recv
            for (auto sock : output) {
                if (sock.GetFd() == listen_sock.GetFd()) {
                    TcpSocket client_sock;
                    sock.Accept(&client_sock);
                    epoll.Add(client_sock);
                } else {
                    std::string req;
                    // TODO: ET模式下必须采用非阻塞的方式进行读写，使用 tcp_socket 的非阻塞读写
                    int n = sock.Recv(&req);
                    if (n < 0) {
                        continue;
                    }
                    if (n == 0) {
                        printf("[client %d] 断开连接\n", sock.GetFd());
                        // 对方关闭
                        sock.Close();  // 其实下面的 Del 也会关闭 sock
                        epoll.Del(sock);
                        continue;
                    }
                    printf("[client %d] %s\n", sock.GetFd(), req.c_str());
                    // 正确读取的情况
                    std::string resp;
                    handler(req, &resp);
                    sock.Send(resp);
                }  // end if else {}
            }  // end for {}
        }  // end while(true)
    }  // end func Start()
};
