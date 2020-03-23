// 可重入 和 线程安全

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define THREAD_NUM 1

pthread_mutex_t mutex;

int g_cnt = 0;

// 这个函数 线程安全
// 但这个函数是 不可重入 函数
void ModifyCount() {
  pthread_mutex_lock(&mutex);
  ++g_cnt;
  printf("before sleep!\n");
  sleep(3);
  printf("after sleep!\n");
  pthread_mutex_unlock(&mutex);
}

void* ThreadEntry(void* args) {
  (void) args;

  for (int i = 0; i < 50000; ++i) {
    ModifyCount();
  }

  return NULL;
}

void MyHandler(int sig) {
  (void) sig;
  ModifyCount();  // !
}

int main() {
  // ctrl + c 产生 2 号信号 SIGINT
  signal(SIGINT, MyHandler);

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

