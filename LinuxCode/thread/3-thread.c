// 多线程提高程序计算性能
// @author: ahojcn

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>

int64_t GetUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

#if 0
// 1. 单线程版本
#define SIZE 100000000

void calc(int* arr, int begin, int end) {
    for (int i = begin; i < end; ++i) {
        arr[i] = arr[i] * arr[i];
    }
}

int main() {
    int* arr = (int*)malloc(sizeof(int) * SIZE);
    int64_t bt = GetUs();
    Calc(arr, 0, SIZE);
    int64_t et = GetUs();
    printf("time used : %ld\n", et-bt);

    return 0;
}
#endif

#if 1
// 2. 多线程版本
#define THREAD_NUM 2
#define SIZE 100000000

typedef struct Arg {
    int begin;
    int end;
    int* arr;
} Arg;


void Calc(int* arr, int begin, int end) {
    for (int i = begin; i < end; ++i) {
        arr[i] = arr[i] * arr[i];
    }
}

void* ThreadEntry(void* args) {
    Arg* p = (Arg*)args;
    Calc(p->arr, p->begin, p->end);
    return NULL;
}

int main() {
    int* arr = (int*)malloc(SIZE * sizeof(int));
    Arg args[THREAD_NUM];
    int base = 0;
    for (int i = 0; i < THREAD_NUM; ++i) {
        args[i].begin = base;
        args[i].end = base + SIZE / THREAD_NUM;
        args[i].arr = arr;
        base += SIZE / THREAD_NUM;
    }

    // 创建多个线程
    pthread_t tid[THREAD_NUM];
    int64_t bt = GetUs();
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_create(&tid[i], NULL, ThreadEntry, &args[i]);
    }
    // 回收线程资源
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(tid[i], NULL);
    }
    int64_t et = GetUs();
    printf("time used: %ld\n", et - bt);
    return 0;
}
#endif