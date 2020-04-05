// 销售火车票
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 10;
pthread_mutex_t mutex;

void* SellTicket(void*);

int main()
{
    // 初始化锁
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, SellTicket, "窗口1");
    pthread_create(&tid2, NULL, SellTicket, "窗口2");
    pthread_create(&tid3, NULL, SellTicket, "窗口3");
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // 释放锁
    pthread_mutex_destroy(&mutex);
    return 0;
}

void*
SellTicket(void* arg)
{
    char* id = (char*) arg;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (ticket > 0) 
        {
            sleep(1);
            --ticket;
            printf("%s 售出 1 张, 剩余 %d 张\n", id, ticket);
            pthread_mutex_unlock(&mutex);
            sched_yield();  // 测试：放弃 CPU 执行权
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            printf("票售罄了!\n");
            break;
        }
    }
}
