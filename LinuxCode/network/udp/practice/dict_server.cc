// 英文 -> 中文 服务器

#include <map>
#include "udp_server.hpp"

int main() {
    std::map<std::string, std::string> m;
    m["hello"] = "你好";
    m["world"] = "世界";

    UdpServer server;
    server.Start("0.0.0.0", 9090, [&m](const std::string& req, std::string* resp){
        if (m.find(req) != m.end()) {
            *resp = m[req];
        } else {
            *resp = "未找到";
        }
    });

    return 0;
}
