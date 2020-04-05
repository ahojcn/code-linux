// 网络版计算器服务端

#include <iostream>
#include <sstream>
#include "tcp_thread_server.hpp"

int main()
{
    TcpThreadServer server;

    // 请求示例："10 20 +"
    // 响应示例："30"
    server.Start("0.0.0.0", 9090, [](const std::string &req, std::string *resp) {
        // 1. 反序列化
        std::stringstream ss(req);
        int num1 = 0, num2 = 0;
        ss >> num1;
        ss >> num2;
        std::string op;
        ss >> op;
        std::cout << "req:" << num1 << num2 << op << std::endl;
        // 2. 计算
        int ret = 0;
        if (op == "+")
        {
            ret = num1 + num2;
        }
        else if (op == "-")
        {
            ret = num1 - num2;
        }
        // 3. 序列化
        // *resp = std::to_string(ret);
        std::stringstream ss_resp;
        ss_resp << ret;
        *resp = ss_resp.str();
    });
    return 0;
}
