// 网络版计算器服务端

#include <sstream>
#include "tcp_client.hpp"

int main()
{
    TcpClient cli;
    bool ret = cli.Connect("127.0.0.1", 9090);
    if (!ret)
    {
        return 1;
    }
    while (true)
    {
        printf("> ");
        fflush(stdout);
        int num1 = 0, num2 = 0;
        char op[10] = {0};
        scanf("%d %d %s", &num1, &num2, op);
        // 序列化数据
        std::stringstream ss;
        ss << num1 << " " << num2 << " " << op;
        // 发送数据
        cli.Send(ss.str());

        std::string resp;
        cli.Recv(&resp);
        printf("resp: %s", resp.c_str());
    }
    return 0;
}
