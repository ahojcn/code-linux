#pragma once
#include <sys/select.h>
#include <vector>
#include <functional>
#include <set>  // 底层红黑树，更适合下面的场景，高效的插入和删除，方便找到最大元素
// #include <unordered_set>  // 底层哈希表
#include "tcp_socket.hpp"

// lhs 左操作数，rhs 右操作数
bool operator<(const TcpSocket& lhs, const TcpSocket& rhs) {
    return lhs.GetFd() < rhs.GetFd();
}

// select 这个函数本身用起来不是很方便，封装成一个类
// 提供以下操作
// 文件描述符管理器，这个管理器中包含的 socket 就是要使用 select 管理的 socket
// 1. 新增一个文件描述符
// 2. 删除一个文件描述符
// 3. 等待操作：等待管理的文件描述符就绪（只考虑读就绪的情况）
class Selector {
public:
    void Add(const TcpSocket& sock) {
        printf("[Selector::Add] %d\n", sock.GetFd());
        socket_set_.insert(sock);
    }

    void Del(const TcpSocket& sock) {
        printf("[Selector::Del] %d\n", sock.GetFd());
        socket_set_.erase(sock);
    }

    // Wait 返回的时候需要告诉调用者哪些文件描述符就绪了
    void Wait(std::vector<TcpSocket>* output) {
        output->clear();
        // 调用 Wait 就相当于调用 select 进行等待
        // 获取最大文件描述符
        if (socket_set_.empty()) {
            printf("[Selector::Wait] error, socket_set_ is empty.\n");
            return;
        }
        int max_fd = socket_set_.rbegin()->GetFd();
        fd_set read_fds;
        FD_ZERO(&read_fds);
        // 把 set 中的每个需要关注的文件描述符都设置到 readfds 之中
        for (const auto& sock : socket_set_) {
            FD_SET(sock.GetFd(), &read_fds);
        }
        // select 默认是阻塞等待，一直到有文件描述符返回的时候才返回
        // 当返回之后就应该根据文件描述符的返回情况构造一个输出参数，告诉调用者哪些 socket 就绪
        int nfds = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (nfds < 0) {
            perror("select");
            return;
        }
        // 已经返回，返回结果同样保存在 read_fds 之中
        for (int fd = 0; fd < max_fd + 1; ++fd) {
            if (FD_ISSET(fd, &read_fds)) {
                // 如果在 read_fds 之中，当前fd就是读就绪
                // 就把这个结果放到输出结果中
                output->push_back(TcpSocket(fd));
            }
        }
    }

private:
    std::set<TcpSocket> socket_set_; // 需要 TcpSocket 支持 operator<
};

// 实现一个 select 版本的 TCP server
typedef std::function<void (const std::string&, std::string*)> Handler;

#define CHECK_RET(exp) \
    if (!exp) { \
        return false; \
    } \

class TcpSelectServer {
public:
    bool Start(const std::string& ip, uint16_t port, Handler handler) {
        // 1. 创建 socket
        TcpSocket listen_sock;
        CHECK_RET(listen_sock.Socket())
        // 2. 绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        // 3. 监听文件描述符
        CHECK_RET(listen_sock.Listen());
        // 4. 创建一个 Selector 对象, 让这个对象先把 listen sock 管理起来
        //    后续进行等待都是靠 selector 对象来完成
        Selector selector;
        selector.Add(listen_sock);
        // 5. 进入主循环
        while (true) {
            // 6. 不再是直接调用 accept，而是使用 selector 进行等待
            std::vector<TcpSocket> output;
            selector.Wait(&output);
            // 7. 遍历返回结果，一次处理每个就绪的 socket
            for (auto tcp_socket : output) {
                // 8. 分成两种情况讨论
                //    8.1 如果就绪的 socket 是 listen_sock 要进行的操作是调用 accept
                //    8.2 如果不是，要调用 recv
                if (tcp_socket.GetFd() == listen_sock.GetFd()) {
                    TcpSocket client_sock;
                    std::string ip;
                    std::uint16_t port;
                    tcp_socket.Accept(&client_sock, &ip, &port);
                    selector.Add(client_sock);
                    printf("[client %s:%d] connected!\n", ip.c_str(), port);
                } else {
                    std::string req;
                    int n = tcp_socket.Recv(&req);
                    if (n < 0) {
                        continue;
                    }
                    if (n == 0) {
                        // 对端关闭，注意要把 socket 从 selector 中删除
                        tcp_socket.Close();
                        selector.Del(tcp_socket);
                        continue;
                    }
                    printf("[client ] %s\n", req.c_str());

                    // 根据请求计算响应
                    std::string resp;
                    handler(req, &resp);
                    // 把响应写回客户端
                    tcp_socket.Send(resp);
                }
            }
        }
    }
private:
};