#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// 互斥锁
pthread_mutex_t mutex;
// 条件变量
pthread_cond_t cond;

// 传球动作
void* ThreadEntry1(void* args) {
    (void) args;
    while (1) {
        printf("传球\n");
        // 传球过去了，通知一下
        pthread_cond_signal(&cond);
        usleep(788789);
    }
    return NULL;
}

// 扣篮动作
void* ThreadEntry2(void* args) {
    (void)args;
    while (1) {
        // 首先得等待球传过来
        // 一直等到球传过来
        pthread_cond_wait(&cond, &mutex);
        printf("-扣篮\n");
        usleep(123123);
    }
    return NULL;
}

int main() {
    // 初始化 cond 和 mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, ThreadEntry1, NULL);
    pthread_create(&tid2, NULL, ThreadEntry2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
