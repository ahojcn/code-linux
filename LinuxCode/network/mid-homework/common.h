#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/select.h>
#include <errno.h>

#define SERVER_PORT 9090
#define MAXLINE 1024
