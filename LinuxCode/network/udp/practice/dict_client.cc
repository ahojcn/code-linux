// 英 -> 汉客户端

#include <iostream>
#include "udp_client.hpp"

int main() {
    UdpClient cli("127.0.0.1", 9090);
    while (true) {
        std::cout << "zh> ";
        std::string zh;
        std::cin >> zh;
        cli.SetReq(zh);
        std::string en;
        cli.GetResp(&en);
        std::cout << "ch: " << en << std::endl;
    }
    return 0;
}
