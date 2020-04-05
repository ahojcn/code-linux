#include <jsoncpp/json/json.h>
#include "tcp_client.hpp"

int main()
{
  TcpClient client;
  bool ret = client.Connect("127.0.0.1", 9090);
  if (!ret) {
    printf("connect failed!\n");
    return 1;
  }
  while (true)
  {
    printf("input> ");
    fflush(stdout);
    int num1, num2;
    char op[10] = {0};
    scanf("%d %d %s", &num1, &num2, op);

    Json::Value req_json;
    req_json["num1"] = num1;
    req_json["num2"] = num2;
    req_json["op"] = op;

    //Json::FastWriter writer;
    Json::StyledWriter writer;
    std::string req = writer.write(req_json);
    printf("req: %s\n", req.c_str());

    client.Send(req);

    std::string resp;
    client.Recv(&resp);

    Json::Reader reader;
    Json::Value resp_json;
    reader.parse(resp, resp_json);
    printf("resp: %s\n", resp.c_str());
  }
  return 0;
}

