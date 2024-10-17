#include "Vector.h"
#include <cmath>

Vector::Vector() : x(0.0), y(0.0), z(0.0) {}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(x - other.x, y - other.y, z - other.z);
}

Vector Vector::operator*(double scalar) const {
    return Vector(x * scalar, y * scalar, z * scalar);
}

double Vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector Vector::normalize() const {
    double mag = magnitude();
    return Vector(x / mag, y / mag, z / mag);
}
