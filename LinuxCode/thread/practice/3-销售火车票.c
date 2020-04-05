// 销售火车票
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 10;

// 每个窗口都执行的售票操作，假设每个窗口每次只卖出 1 张
void* SellTicket(void*);

int main()
{
    // 有 3 个售票窗口在售票
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, SellTicket, "窗口1");
    pthread_create(&tid2, NULL, SellTicket, "窗口2");
    pthread_create(&tid3, NULL, SellTicket, "窗口3");
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}

void*
SellTicket(void* arg)
{
    char* id = (char*) arg;
    while (1)
    {
        if (ticket > 0) 
        {
            sleep(1);  // 售票员小姐姐操作一下
            --ticket;
            printf("%s 售出 1 张, 剩余 %d 张\n", id, ticket);
        }
        else
        {
            printf("票售罄了!\n");
            break;  // 关闭售票窗口
        }
    }
}
