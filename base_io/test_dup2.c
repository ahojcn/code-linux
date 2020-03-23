// 
// dup2 函数重定向
//


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
  int fd = open("./out.txt", O_APPEND | O_WRONLY);
  printf("%d\n", fd);
  dup2(fd, 1);
  fprintf(stdout, "lalala\n");

  return 0;
}
