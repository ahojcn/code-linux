// 
// 向共享内存写数据
//


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>  // shmget()
#include <sys/types.h>


int main() {
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

  // 3. 使用
  strcpy(p, "hehe\n");

  return 0;
}
