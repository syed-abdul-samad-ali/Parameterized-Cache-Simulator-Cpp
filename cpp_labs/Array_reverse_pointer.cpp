#include <iostream>

void reverse_array(int *arr, int size) {
    int *start = arr;
    int *end = arr + size - 1;

    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;

    reverse_array(arr, size);

    for (int i = 0; i < size; i++) {
        std::cout << *(arr + i) << " ";
    }
    std::cout << std::endl;

    return 0;
}