#include <iostream>
#include <utility>

class Buffer {
private:
    int* data;
    size_t size;

public:
    Buffer(size_t s) : size(s), data(new int[s]) {}

    ~Buffer() {
        delete[] data;
    }

    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = delete;
};

int main() {
    Buffer b1(100);
    Buffer b2 = std::move(b1);

    return 0;
}