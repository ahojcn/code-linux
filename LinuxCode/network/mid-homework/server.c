// 网络编程实战-期中作业-服务端
#include "common.h"

char *run_cmd(char *cmd) {
    char *data = malloc(16384);
    bzero(data, sizeof(data));
    FILE *fdp;
    const int max_buffer = 256;
    char buffer[max_buffer];
    fdp = popen(cmd, "r");
    char *data_index = data;
    if (fdp) {
        while (!feof(fdp)) {
            if (fgets(buffer, max_buffer, fdp) != NULL) {
                int len = strlen(buffer);
                memcpy(data_index, buffer, len);
                data_index += len;
            }
        }
        pclose(fdp);
    }
    return data;
}

int main() {
    // 1. 创建 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        return -1;
    }

    // 2. 初始化 ip 和 port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // 3. 设置如果是处于 TIME_WAIT 状态的 fd 可以使用
    int on = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 4. 绑定 ip 和 port
    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind");
        return -1;
    }

    // 5. 监听
    ret = listen(listen_fd, 128);
    if (ret < 0) {
        perror("listen");
        return -1;
    }

    // 6. 注册事件处理函数
    signal(SIGPIPE, SIG_IGN);

    int count = 0;
    // 7. 进入事件处理
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if (conn_fd < 0) {
            perror("accept");
            return -1;
        }

        while (1) {
            char req[1024] = {0};
            int n = read(conn_fd, req, sizeof(req));
            if (n < 0) {
                perror("read");
                return -1;
            } else if (n == 0) {
                printf("[%s:%d] client closed\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                close(conn_fd);
                break;
            } else {
                count ++;
                req[n] = '\0';
                if (strncmp(req, "ls", n) == 0) {
                    char* res = run_cmd("ls");
                    if (send(conn_fd, res, strlen(res), 0) < 0) {
                        perror("send");
                        return -1;
                    }
                    free(res);
                } else if (strncmp(req, "pwd", n) == 0) {
                    char res[256] = {0};
                    if (getcwd(res, sizeof(res)) == NULL) {
                        strcpy(res, "getcwd error!");
                    }
                    if (send(conn_fd, res, strlen(res), 0) < 0) {
                        perror("send");
                        return -1;
                    }
                } else if (strncmp(req, "cd ", 3) == 0) {
                    char target_path[256] = {0};
                    memcpy(target_path, req + 3, strlen(req) - 3);
                    char res[256] = {0};
                    if (chdir(target_path) == -1) {
                        strcpy(res, "chdir error!");
                    } else {
                        strcpy(res, "chdir ok!");
                    }
                    if (send(conn_fd, res, strlen(res), 0) < 0) {
                        perror("send");
                        return -1;
                    }
                } else {
                    char* res = "error: unknow input command!";
                    if (send(conn_fd, res, strlen(res), 0) < 0) {
                        perror("send");
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}
