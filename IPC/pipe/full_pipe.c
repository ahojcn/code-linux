// 
// 测试管道的容量
//


#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main() {
  int fd[2];
  int ret = pipe(fd);
  if (ret < 0) {
    perror("pipe");
    return 1;
  }

  int cnt = 0;
  while(1) {
    write(fd[1], "a", 1);
    printf("cnt: %d\n", cnt++);  // 65535, 2 个字节, 64k
  }

  close(fd[0]);
  close(fd[1]);

  return 0;
}
