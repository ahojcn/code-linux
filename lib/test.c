#include <stdio.h>


extern int add(int x, int y);


int main() {
  int ret = add(1, 2);
  printf("%d\n", ret);

  return 0;
}
