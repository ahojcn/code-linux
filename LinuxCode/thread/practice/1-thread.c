// 多线程 CPU 使用量测试

#include <stdio.h>
#include <pthread.h>

#if 0
int main()
{
    while (1)
        ;
    return 0;
}
#endif

#if 1
void*
ThreadEntry(void* args)
{
    (void) args;
    while (1)
        ;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, ThreadEntry, NULL);
    pthread_create(&tid2, NULL, ThreadEntry, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
#endif