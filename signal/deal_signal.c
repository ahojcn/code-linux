// 
// signal 处理信号
//


#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void handler(int signo) {
  printf("signo = %d\n", signo);
}


int main() {
  signal(2, handler);  // ctrl + c
  signal(3, handler);  // ctrl + '\'
  signal(8, SIG_IGN);
  while (1)
    sleep(1);

  return 0;
}
