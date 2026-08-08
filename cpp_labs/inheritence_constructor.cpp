#include <iostream>

class Base {
private:
    int i_;
    int j_;

public:
    Base() : i_(0), j_(0) {}
    Base(int i) : i_(i), j_(0) {}
    Base(int i, int j) : i_(i), j_(j) {}

    void print() const {
        std::cout << "i: " << i_ << ", j: " << j_ << std::endl;
    }
};

class Derived : public Base {
public:
    using Base::Base;
};

int main() {
    Derived d1(42);
    Derived d2(10, 20);

    d1.print();
    d2.print();

    return 0;
}