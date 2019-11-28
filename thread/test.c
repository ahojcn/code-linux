// 
// 多线程测试
// 1. 创建线程
// 2. 终止线程
// 3. 等待线程
// 4. 线程分离
// 线程控制相关函数不是系统调用
// POSIX 线程库
// POSIX 是一个标准，约定一个操作系统都需要提供哪些接口
// pthread => POSIX thread lib
// C++11 内置了线程库
//


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void* func(void* arg) {
  (void) arg;
  while (1) {
    sleep(1);
    printf("in func thread, %lu\n", pthread_self());
  }
}


int main() {
  pthread_t tid = 0;
  pthread_create(&tid, NULL, func, NULL);
  while (1) {
    printf("in main thread, %lu\n", pthread_self());
    sleep(1);
  }

  return 0;
}

