#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
    double x, y, z;
    Vector();
    Vector(double x, double y, double z);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(double scalar) const;
    double magnitude() const;
    Vector normalize() const;
};

#endif /* VECTOR_H */
