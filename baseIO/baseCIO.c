#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // write
    FILE* fp = fopen("file.out", "w");
    if (fp == NULL) {
        perror("fopen error");
        exit(-1);
    }

    const char* msg = "hello file io.\n";
    for (size_t i = 0; i < 5; i++) {
        fwrite(msg, strlen(msg), 1, fp);
    }
    
    fclose(fp);

    // read
    fp = fopen("file.out", "r");
    if (fp == NULL) {
        perror("fopen error");
        exit(-1);
    }

    char buf[1024] = {0};
    size_t ret = fread(buf, strlen(msg), 5, fp);
    printf("ret = %zu\nbuf = %s\n", ret, buf);
    // for (size_t i = 0; i < strlen(buf); i++) {
    //     printf("buf[%lu] : %c\n", i, buf[i]);
    // }

    const char* str = "print message to screen.\n";
    printf("%s", str);
    fwrite(str, strlen(str), 1, stdout);
    fprintf(stdout, "%s", str);

    return 0;
}