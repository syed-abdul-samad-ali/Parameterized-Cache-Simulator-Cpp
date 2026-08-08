#include <iostream>

class Vector {
private:
    double x_;
    double y_;

public:
    Vector() : x_(0.0), y_(0.0) {}
    Vector(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }
};

Vector operator+(const Vector& u, const Vector& v) {
    return Vector(u.x() + v.x(), u.y() + v.y());
}

double operator*(const Vector& u, const Vector& v) {
    return u.x() * v.x() + u.y() * v.y();
}

int main() {
    Vector u(1.0, 2.0);
    Vector v(3.0, 4.0);

    Vector w = u + v;
    double d = u * v;

    std::cout << "w.x: " << w.x() << ", w.y: " << w.y() << std::endl;
    std::cout << "Dot Product: " << d << std::endl;

    return 0;
}