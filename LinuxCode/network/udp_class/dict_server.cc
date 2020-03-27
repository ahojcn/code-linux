// 中英文翻译服务器
// 用户输入一个英文单词，返回一个对应中文结果

#include <unordered_map>
#include "udp_server.hpp"

int main() {
    UdpServer server;

    std::unordered_map<std::string, std::string> dict;
    dict.insert(std::make_pair("hello", "你好"));
    dict.insert(std::make_pair("world", "世界"));

    // [] 中可以写东西，把 lambda 外部的变量捕获到 lambda 表达式里面
    // 捕获可以按值捕获，也可以按引用捕获
    server.Start("0.0.0.0", 9090, [&dict](const std::string& req, std::string* resp) {
        auto it = dict.find(req);
        if (it == dict.end()) {
            *resp = "未找到结果";
        } else {
            *resp = it->second;
        }
    });

    return 0;
}
