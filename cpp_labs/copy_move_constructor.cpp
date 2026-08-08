#include <iostream>
#include <utility>

class Vector {
private:
    double x_;
    double y_;

public:
    Vector() : x_(0.0), y_(0.0) {}

    Vector(const Vector& v) : x_(v.x_), y_(v.y_) {}

    Vector(Vector&& v) : x_(v.x_), y_(v.y_) {}
};

int main() {
    Vector v;
    Vector w(v);
    Vector u(std::move(v));

    return 0;
}