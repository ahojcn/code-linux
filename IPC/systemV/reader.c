// 
// 从共享内存读数据
//


#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>  // shmget()
#include <sys/types.h>


int main() {
  // 1. 创建 & 打开共享内存对象
  key_t key = ftok(".", 0x1);
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  int ret = shmget(key, 1024, IPC_CREAT | 0666);
  if (ret < 0) {
    perror("shmget");
    return 1;
  }

  // 2. 附加到共享内存上
  char* p = (char*)shmat(ret, NULL, 0);
  
  // 3. 直接使用
  printf("reader: %s", p);

  return 0;
}
