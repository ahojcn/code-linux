// 线程控制的相关函数
// 1. 创建线程
// pthread_create();

// 2. 终止线程
// pthread_exit(NULL);  // 结束线程
// exit(0);  // 结束整个进程
// pthread_cancel(tid);  // 结束本进程中的任意线程

// 3. 等待线程
// pthread_join(tid, NULL);  // 阻塞

// 4. 线程分离
// pthread_detach(tid);

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* ThreadEntry(void* arg) {
    (void) arg;
    while (1) {
        printf("In ThreadEntry, %lu\n", pthread_self());
        sleep(1);
    }
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, ThreadEntry, NULL);
    while (1) {
        printf("In Main Thread, %lu\n", pthread_self());
        sleep(1);
    }

    return 0;
}
