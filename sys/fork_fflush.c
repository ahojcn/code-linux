// 
// fork 和 缓冲区
//

#include <stdio.h>
#include <unistd.h>

int main() {

  for (int i = 0; i < 2; ++i) {
    fork();
    printf("=");
    fflush(stdout);
  }
  return 0;
}
