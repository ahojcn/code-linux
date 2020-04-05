#include "tcp_thread_server.hpp"

void
HttpHandler(const std::string& req, std::string* resp)
{
    (void)req;
    // std::string first_line = "HTTP/1.1 200 OK\n";
    // std::string body = "<html><body>hello world</body></html>";
    //std::string header = "Content-Type: text/html\nContent-Length:" + std::to_string(body.size()) + "\n";
    // *resp = first_line + header + "\n" + body;

    std::string first_line = "HTTP/1.1 302 Found\n";
    std::string header = "Location: http://ahoj.cc\n";
    *resp = first_line + header + "\n";
}

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0", 9091, HttpHandler);
    return 0;
}
