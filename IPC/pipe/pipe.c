// 
// 进程间通信 - 管道
// pipe()
//


#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main() {
  // 使用 pipe 创建一对文件描述符
  // 通过这一对文件描述符操作内核中的管道
  int fd[2];
  int ret = pipe(fd);
  if (ret < 0) {
    perror("pipe");
    return 1;
  }

  // fd[0] -> 读数据
  // fd[1] -> 写数据
  // char buf[1024] = "hehe";
  // write(fd[1], buf, strlen(buf));
  // char buf_out[1024] = {0};
  // int n = read(fd[0], buf_out, sizeof(buf_out) - 1);
  // buf_out[n] = '\0';
  // printf("%s\n", buf_out);

  // fork 后，两个进程都有了 fd，指向同一块内核中的管道
  // 管道中的数据读到之后就出队列了
  // 多个进程同时尝试去读，只有一个进程能读到数据，其他进程读不到
  // 管道内置了同步互斥机制，不会出现一人读一半数据的情况
  // 如果管道为空，尝试读，就会在 read 函数处阻塞
  // 如果管道满了，尝试写，就会在 write 函数处阻塞
  // 管道里面能装多少数据？
  // 65535，占用 2 个字节，64 k，太小了
  ret = fork();
  if (ret > 0) {
    // father
    // write pipe 
    char buf[1024] = "hehe";
    write(fd[1], buf, strlen(buf));

    sleep(1);

    char buf_out[1024] = { 0 };
    int n = read(fd[0], buf_out, sizeof(buf_out) - 1);
    buf_out[n] = '\0';
    printf("father read %s\n", buf_out);
  } else if (ret == 0) {
    // child
    // read pipe 
    char buf_out[1024] = { 0 };
    int n = read(fd[0], buf_out, sizeof(buf_out) - 1);
    buf_out[n] = '\0';
    printf("child read %s\n", buf_out);
  } else {
    perror("fork");
  }

  // 管道使用完后需要及时关闭
  close(fd[0]);
  close(fd[1]);

  return 0;
}
