// 线程栈分配测试

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void*
ThreadEntry(void* args)
{
    (void) args;
    while (1)
    {
        sleep(1);
    }
}

int main()
{
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, ThreadEntry, NULL);
    pthread_create(&tid2, NULL, ThreadEntry, NULL);
    pthread_create(&tid3, NULL, ThreadEntry, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    return 0;
}
