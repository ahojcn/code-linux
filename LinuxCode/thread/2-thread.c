
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *ThreadFunc(void *args)
{
  (void)args;
  while (1)
  {
    // sleep(1);
    ;
  }
  return NULL;
}

int main()
{
  pthread_t pid1, pid2, pid3;
  pthread_create(&pid1, NULL, ThreadFunc, NULL);
  pthread_create(&pid2, NULL, ThreadFunc, NULL);
  pthread_create(&pid3, NULL, ThreadFunc, NULL);

  while (1)
  {
    // sleep(1);
    ;
  }

  return 0;
}
