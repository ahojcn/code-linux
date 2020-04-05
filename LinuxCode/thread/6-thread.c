// 同步   使用条件变量

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <signal.h>

// 互斥锁
pthread_mutex_t mutex;
// 条件变量
pthread_cond_t cond;

int g_cnt = 0;

void* ThreadEntry1(void* args) {
  (void) args;
  while (1) {
    printf("传球\n");
    // 传球
    pthread_cond_signal(&cond);
    usleep(789789);
  }
  return NULL;
}
void* ThreadEntry2(void* args) {
  (void) args;
  while (1) {
    // 等待球传过来，才能扣篮
    // 一直等待到  传球的通知到来
    // 1. 先释放锁  2. 等待条件就绪  3. 重新获取锁，准备执行后续操作     
    // 1,2 操作必须是原子性的，否则可能错过其他线程通知消息，导致还在这里傻等
    // 大部分情况下，条件变量得和互斥锁一起使用
    pthread_cond_wait(&cond, &mutex);
    printf("扣篮\n");
    usleep(123123);
  }
  return NULL;
}

// 模拟传球和扣篮
// 先传球、再扣篮
int main() {
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, ThreadEntry1, NULL);
  pthread_create(&tid2, NULL, ThreadEntry2, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}

