// 
// fopen fread fwrite fclose
// open read write close
//


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main() {
  // open 的返回值是一个文件描述符
  // 0 -> stdin
  // 1 -> stdout
  // 2 -> stderr
  int fd = open("./out.txt", O_WRONLY | O_CREAT);
  printf("fd = %d\n", fd);
  // printf("stdin = %d, stdout = %d, stderr = %d\n", 
      // stdin->_fileno, stdout->_fileno, stderr->_fileno);

  if (fd < 0) {
    perror("open");
    return 1;
  }

  // 读文件
  // char buf[1024] = { 0 };  // first check
  // ssize_t cnt = read(fd, buf, sizeof(buf) - 1);
  // buf[cnt] = '\0';  // double check
  // printf("buf:\n%s", buf);


  // 写文件
  char buf[1024] = "hehe";
  ssize_t n = write(fd, buf, strlen(buf));
  printf("n = %ld\n", n);
  perror("write");

  close(fd);
  // vim 操作
  // 给一个单词加括号
  // 1. v, e 选中单词
  // 2. shift + s 进行 surround 操作
  // 3. ) 或者 (
  // 依赖 vim-surround
  return 0;
}
