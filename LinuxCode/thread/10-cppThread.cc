#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

// 信号量、读写锁目前 cpp 还没有
//
// cpp 提供了原子操作:本质上也是锁，只不过是 CPU 指令级别的锁，本质是自旋锁
#include <atomic>

std::mutex lock;

std::atomic_int count(0);

void Handler() {
  for (int i = 0; i < 50000; ++i) {
    ++count;
  }
}

int main() {
  // std::thread 构造函数用到的语法：变长参数模板
  std::thread t(Handler);
  std::thread t2(Handler);
  // t.detach();
  t.join();
  t2.join();
  printf("count:%d\n", int(count));

  return 0;
}
