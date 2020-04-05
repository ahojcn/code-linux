#include <stdio.h>
#include <assert.h>

void
Swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void
BubbleSort(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int isSorted = 1;

        for (int j = 0; j < size - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                Swap(arr + j, arr + j + 1);
                isSorted = 0;
            }
        }

        if (isSorted == 1) {
            break;
        }
    }
}

int main() {
    int arr[] = {3,2,4,6,9,10};
    BubbleSort(arr, sizeof(arr)/sizeof(arr[0]));
    for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
