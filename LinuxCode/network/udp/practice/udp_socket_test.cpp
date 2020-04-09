#include <iostream>

#include "udp_socket.hpp"

int main() {
    UdpSocket socket;
    bool ret = socket.Create();
    if (!ret) {
        perror("Socket::Create()");
        return -1;
    }
    
    while (true) {
        std::string input;
        std::cout << "input> ";
        std::cin >> input;

        ret = socket.SendTo(input, "127.0.0.1", 8080);
        if (!ret) {
            perror("Socket::SendTo()");
            continue;
        }
        
        std::string resp;
        ret = socket.RecvFrom(&resp);
        if (!ret) {
            perror("Socket::RecvFrom()");
            continue;
        }
    }

    socket.Close();
    return 0;
}
