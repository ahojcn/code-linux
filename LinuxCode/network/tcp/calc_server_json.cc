#include <jsoncpp/json/json.h>
#include "tcp_thread_server.hpp"

int main()
{
    TcpThreadServer server;
    server.Start("127.0.0.1", 9090, [](const std::string &req, std::string *resp) {
        // 1. 将 req 反序列化
        Json::Reader reader;  // 把字符串转换成 Json::Value
        Json::Value req_json;  // 用来表示一个 Json 数据
        // Json::Writer 把Json::Value 转成字符串
        reader.parse(req, req_json);

        // 2. 计算
        int num1 = req_json["num1"].asInt();
        int num2 = req_json["num2"].asInt();
        std::string op = req_json["op"].asString();
        int result = 0;
        if (op == "+") {
            result = num1 + num2;
        } else if (op == "-") {
            result = num1 - num2;
        }

        // 3. 返回响应
        Json::FastWriter writer;
        Json::Value resp_json;
        resp_json["result"] = result;
        *resp = writer.write(resp_json);
    });
    return 0;
}
