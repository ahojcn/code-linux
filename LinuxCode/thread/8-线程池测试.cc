
#include <unistd.h>
#include "threadpool.hpp"

class MyTask : public Task {
public:
  MyTask(int id) : id_(id) {
  }

  void Run() {
    printf("id = %d\n", id_);
  }
private:
  int id_;
};

int main() {
  ThreadPoll pool(10);
  for (int i = 0; i < 20; ++i) {
    pool.AddTask(new MyTask(i));
  }
  while (1) {
    sleep(1);
  }
  return 0;
}
