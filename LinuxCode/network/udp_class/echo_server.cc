#include "udp_server.hpp"

// void Echo(const std::string& req, std::string* resp) {
//     *resp = req;
// }

int main() {
    UdpServer server;
    // lambda 表达式本质上是一个匿名函数，C++11
    // []() -> int {}  // int 是返回值类型  // [] 里面可以放东西
    server.Start("0.0.0.0", 9090, [](const std::string& req, std::string* resp) {
        *resp = req;
    });
    return 0;
}
