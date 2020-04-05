#include <stdio.h>

int
BinarySearch(int* arr, int size, int val) {
    int left = 0, right = size;
    while (left < right) {
        int mid = (right - left) / 2 + left;
        if (arr[mid] == val) {
            return mid;
        } else if (arr[mid] > val) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}

int main() {
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    int r = BinarySearch(arr, sizeof(arr)/sizeof(arr[0]), 2);
    printf("%d\n", r);
    return 0;
}
