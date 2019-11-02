// 
// 进程退出
//

#include <stdio.h>
#include <stdlib.h>  // exit()
#include <unistd.h>  // _exit()

void when_main_exit() {
  // 这是一个回调函数
  // 在 return 和 exit() 都会执行，但是 _exit() 退出不会执行
  printf("\nbye\n");
  // 如果程序中打开了一些临时文件，在这个函数中可以关闭这些文件
}

int main() {
  printf("haha");
  // _exit(1);

  atexit(when_main_exit);
  // exit(-1);  // 会比 _exit 多一些操作, 关闭流 刷新缓冲区
  return 0;  // 也会刷新缓冲区
}
