// 
// 一个简易的 shell 程序
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * 切分命令和参数
 * input: 带切分命令
 * output: 切分结果
 * return: 包含了几个有效元素
 */ 
int split(char input[], char* output[]);


int main() {
  while (1) {
    // 1. 打印提示符
    printf("myshell-> ");
    fflush(stdout);
    // 2. 用户输入一个指令
    char command[1024] = { 0 };
    gets(command);
    // 3. 解析命令，把要执行的命令和参数分割
    char* argv[1014] = { 0 };
    split(command, argv);
  }

  return 0;
}  // end of main()


int split(char input[], char* output[]) {
  // 使用 strtok 实现
  // 注意 strtok 多线程环境下会导致线程不安全
  // 在 C++ 中有一个 #include<sstream> 或者 boost boost::split(更科学) 中有更优雅的切割方法
  char* p = strtok(input, " ");
  int i = 0;
  while (p != NULL) {
    output[i] = p;
    ++i;
    p = strtok(NULL, " ");
  }

  return i;
}  // end of split()

