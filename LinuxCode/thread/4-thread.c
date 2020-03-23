// 线程互斥
// 给临界资源操作加互斥锁

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 2

// 创建全局互斥量/互斥锁
pthread_mutex_t mutex;

int g_cnt = 0;

void* ThreadEntry(void* args) {
  (void) args;

  for (int i = 0; i < 50000; ++i) {
    // 上锁
    pthread_mutex_lock(&mutex);

    ++g_cnt;
    
    // 解锁
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

int main() {

  // 初始化锁
  pthread_mutex_init(&mutex, NULL);

  pthread_t tid[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_create(&tid[i], NULL, ThreadEntry, NULL);
  }

  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(tid[i], NULL);
  }
  printf("g_cnt : %d\n", g_cnt);

  // 释放锁
  pthread_mutex_destroy(&mutex);

  return 0;
}

