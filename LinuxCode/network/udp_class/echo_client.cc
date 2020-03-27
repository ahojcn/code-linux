#include "udp_client.hpp"
#include <iostream>

int main() {
    UdpClient cli("127.0.0.1", 9090);

    while (true) {
        std::string s, resp;
        std::cout << "input: ";
        std::cin >> s;
        cli.SendTo(s);
        cli.RecvFrom(&resp);
        std::cout << "resp: " << resp << std::endl;
    }

    return 0;
}
