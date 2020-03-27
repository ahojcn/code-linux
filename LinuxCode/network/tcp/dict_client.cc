#include <iostream>
#include "tcp_client.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage ./dict_client [ip]" << std::endl;
        return 1;
    }
    // 从标准输入读取数据
    // 读到的数据发给服务器
    // 读取服务器返回的响应
    // 输出响应
    TcpClient cli;

    bool ret = cli.Connect(argv[1], 9090);
    if (!ret) {
        std::cout << "连接失败" << std::endl;
        return 1;
    }

    while (true) {
        std::string req, resp;
        std::cout << "输入单词：";
        std::cin >> req;
        cli.Send(req);
        cli.Recv(&resp);
        std::cout << "resp：" << resp << std::endl;
    }

    return 0;
}
