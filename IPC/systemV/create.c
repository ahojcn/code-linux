// 
// 创建一个共享内存对象
//


#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>  // shmget()
#include <sys/types.h>


int main() {
  key_t key = ftok(".", 0x1);
  if (key == -1) {
    perror("ftok");
    return 1;
  }
  printf("key: %d\n", key);

  int ret = shmget(key, 1024, IPC_CREAT | 0666);  // 最后一个数字是权限 666 类似于对应文件权限
  if (ret < 0) {
    perror("shmget");
    return 1;
  }
  printf("ret: %d\n", ret);

  return 0;
}
