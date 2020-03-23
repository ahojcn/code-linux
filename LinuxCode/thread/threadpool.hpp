#pragma once 

#include <stdio.h>
#include "BlockingQueue.hpp"

class Task {
public:
  virtual void Run() {
    printf("base run\n");
  }

  virtual ~Task() {}
};

// 线程池对象启动的时候会创建一组线程
// 每个线程都需要完成一定的任务(执行一些代码，这个逻辑是由调用者来决定的)
class ThreadPoll {
public:
  ThreadPoll(int n) :queue_(100), worker_count_(n) {
    // 创建出 n 个线程
    for (int i = 0; i < worker_count_; ++i) {
      pthread_t tid;
      pthread_create(&tid, NULL, ThreadEntry, this);
      workers_.push_back(tid);
    }
  }

   ~ThreadPoll() {
    // 让线程退出然后再回收
    for (size_t i = 0; i < workers_.size(); ++i) {
      pthread_cancel(workers_[i]);
    }
    for (size_t i = 0; i < workers_.size(); ++i) {
      pthread_join(workers_[i], NULL);
    }
  }

  // 使用线程池的时候由调用者加入一些任务
  void AddTask(Task* task) {
    queue_.Push(task);
  }
private:
  BlockingQueue<Task*> queue_;
  int worker_count_;
  std::vector<pthread_t> workers_;

  static void* ThreadEntry(void* args) {
    ThreadPoll* pool = (ThreadPoll*) args;
    while (true) {
      Task* task = NULL;
      pool->queue_.Pop(&task);
      task->Run();
      delete task;
    }
  }
};
