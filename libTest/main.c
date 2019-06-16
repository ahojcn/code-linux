#include <stdio.h>
#include "sub.h"
#include "add.h"

int main(void) {
    int a = 10, b = 10;
    printf("add(20, 10) = %d\n", add(a, b));
    printf("sub(20, 10) = %d\n", sub(a, b));
    return 0;
}