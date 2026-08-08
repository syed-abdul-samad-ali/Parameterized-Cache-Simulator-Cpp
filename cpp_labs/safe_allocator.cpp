#include <iostream>
#include <cstdlib>
#include <cassert>

void allocate_int(int **ptr, int value) {
    *ptr = (int *)malloc(sizeof(int));
    if (*ptr != nullptr) {
        **ptr = value;
    }
}

int main() {
    int *p = nullptr;

    allocate_int(&p, 42);
    assert(p != nullptr && *p == 42);

    std::cout << "Value: " << *p << std::endl;

    free(p);

    return 0;
}
