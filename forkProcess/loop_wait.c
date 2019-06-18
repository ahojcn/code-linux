#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    for (size_t i = 0; i < 5; i++) {
        pid_t ret = fork();
        if (ret == 0) {
            break;
        }
    }

    return 0;
}