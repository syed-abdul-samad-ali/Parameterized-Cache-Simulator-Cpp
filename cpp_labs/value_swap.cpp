#include <iostream>
#include <cassert>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10;
    int y = 20;

    swap(&x, &y);
    assert(x == 20);

    std::cout << "x: " << x << ", y: " << y << std::endl;

    return 0;
}