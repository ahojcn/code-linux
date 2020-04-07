// 生产者消费者模型解决死锁问题

#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <pthread.h>

# if 1
// 生产者消费者交易场所
std::vector<int> data;

pthread_mutex_t mutex;
pthread_cond_t cond;

// 两个线程代表生产者、消费者
void* Product(void* args) {
  (void) args;
  // 生产数据到 data
  int count = 0;
  while (1) {
    pthread_mutex_lock(&mutex);
    data.push_back(++count);
    printf("Product: %d\n", count);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);  // 通知消费者，已经有数据了
    usleep(789789);
  }
  return NULL;
}

void* Consume(void* args) {
  (void) args;
  // 消费数据
  while (1) {
    pthread_mutex_lock(&mutex);
    // 这里最好是 while 而不是 if
    // pthread_cond_wait 返回不一定是其他线程给的 signal 
    // 这个函数也可能被信号打断
    while (data.empty()) {
      // 加上 wait 后最大的意义就是 消费者线程不必要进行空转，节省资源
      // 1, 2 必须是原子的
      pthread_cond_wait(&cond, &mutex);  // 1. 释放 mutex  2. 等待通知  3. 如果条件就绪，重新获取 mutex
    }
    int res = data.back();
    printf("Consume: %d\n", res);
    data.pop_back();
    pthread_mutex_unlock(&mutex);
    usleep(123123);
  }
  return NULL;
}

int main() {
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, Product, NULL);
  pthread_create(&tid2, NULL, Consume, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
  return 0;
}
#endif  // stack 表示生产者消费者模型

#if 0
#include <semaphore.h>
#include "BlockingQueue.hpp"

BlockingQueue<int> queue(100);

// 进程间通信的管道本质上就是一个 BlockingQueue

void* Product(void* args) {
  (void) args;
  int count = 0;
  while (1) {
    queue.Push(++count);
    printf("Product: %d\n", count);
    usleep(789789);
  }
  return NULL;
}
void* Consume(void* args) {
  (void) args;
  while (1) {
    int count = 0;
    queue.Pop(&count);
    printf("Consume: %d\n", count);
    usleep(123231);
  }
  return NULL;
}

int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, Product, NULL);
  pthread_create(&tid2, NULL, Consume, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
#endif  // 队列表示生产者消费者模型，同步互斥不一定非得使用互斥锁和条件变量来实现，使用信号量来完成(信号量就是一个计数器，表示可用资源个数（P 申请资源，计数器-1，V申请资源，计数器+1，当计数器为0的时候，再去P操作就会阻塞）)
