#pragma once

// 阻塞队列

#include <vector>
#include <pthread.h>
#include <semaphore.h>

// 一般 BlockingQueue 都要求长度有上限的
// 如果队列为空，Pop 会阻塞
// 如果队列满了，Push 会阻塞

// 信号量表示可用资源的个数
// 一个信号量表示当前队列中元素的个数
// 另一个表示当前队列中空格的个数
// 插入元素就是在消耗一个空格资源，插入完成释放了一个元素资源
// 删除元素就是消耗了一个元素资源，释放一个空格资源

// 信号量表示互斥，P、V操作在同一个函数中
// 如果表示同步操作，P、V操作在不同函数中
// 信号量的 +1 -1 是原子的

template <typename T>
class BlockingQueue {
public:
  BlockingQueue(int max_size) : max_size_(max_size), head_(0), tail_(0), size_(0), queue_(max_size) {
    sem_init(&lock_, 0, 1);
    sem_init(&elem_, 0, 0);
    sem_init(&blank_, 0, max_size);
  }

  ~BlockingQueue() {
    sem_destroy(&lock_);
    sem_destroy(&elem_);
    sem_destroy(&blank_);
  }

  void Push(const T& data) {
    // 每次插入元素，就得先申请一个空格资源
    // 如果没有空格资源，说明队列慢了，满了就不能继续插入，并且在 Push 阻塞
    sem_wait(&blank_);

    sem_wait(&lock_);
    queue_[tail_] = data;
    ++tail_;
    ++size_;
    sem_post(&lock_);

    sem_post(&elem_);
  }

  void Pop(T* data) {
    // 每次出队列现申请一个元素资源，如果没有元素资源，就不能出队列，需要阻塞
    sem_wait(&elem_);

    sem_wait(&lock_);
    *data = queue_[head_];
    ++head_;
    --size_;
    sem_post(&lock_);

    sem_post(&blank_);
  }

private:
  std::vector<T> queue_;
  int size_;
  int head_;
  int tail_;
  int max_size_;
  sem_t lock_;  // 二元信号量表示互斥锁（非0即1）
  sem_t elem_;  // 可用元素的个数
  sem_t blank_; // 可用空格的个数
};
