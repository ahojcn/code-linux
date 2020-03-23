#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int n = 0;
    scanf("%d", &n);
    
    char **strs = (char**)malloc(n * sizeof(char*));
    int **tmp = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; ++i) {
        char *tmpstr = (char*) malloc(1000000 * sizeof(char));
        scanf("%s", tmpstr);
        int len = strlen(tmpstr);
        strs[i] = (char*)malloc(len * sizeof(char));
        strcpy(strs[i], tmpstr);
        tmp[i] = (int*)malloc(2 * sizeof(int));
        tmp[i][0] = strs[i][0];
        tmp[i][1] = len;
    }

    int max_len = 0;
    for (int i = 0; i < n; ++i) {
        int tmpmax = tmp[i][1];
        for (int j = 0; i < n; ++j) {
            if (tmp[i][0] == tmp[j][0] && tmp[i][1] < tmp[j][1]) {
                tmpmax = tmp[j][1];
            }
        }
        max_len += tmpmax;
    }

    return 0;
}