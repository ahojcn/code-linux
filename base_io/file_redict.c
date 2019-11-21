// 
// 重定向
// 底层实现，依赖操作系统的文件描述符表的分配规则
//


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
  close(1);  // closed 1, stdout
  int fd = open("out.txt", O_WRONLY);
  fprintf(stderr, "fd = %d\n", fd);

  printf("new print %d\n", stdout->_fileno);
  fprintf(stdout, "new fprintf %d\n", stdout->_fileno);

  return 0;
}
